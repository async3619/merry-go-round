// @ts-ignore
import * as flatten from "lodash.flatten";

import {
    bindingPath,
    bindingTemplatePath,
    DependencyMetadata,
    DependencyMetadataItem,
    dependencyPath,
    getDependencyName,
    isDebug,
} from "./utils";
import * as path from "path";
import * as fs from "fs-extra";

function compileDependencyMetadata(metadataItems: DependencyMetadataItem[], dependencyDirectory: string): string[] {
    return metadataItems
        .map<string | null>(metadataItem => {
            if (Array.isArray(metadataItem)) {
                const [condition, positive, negative] = metadataItem;
                const [prefix, postfix] = condition.split("=");
                let passed = false;

                if (prefix.toLowerCase() === "os") {
                    let os = postfix;
                    if (os === "windows") os = "win32";

                    passed = process.platform === os;
                }

                return passed ? positive : negative;
            }

            return metadataItem;
        })
        .filter(meta => meta)
        .map(itemPath => path.resolve(dependencyDirectory, itemPath));
}
export async function installDependency(dependencies: string[]) {
    const metadataPromises = dependencies.map<Promise<DependencyMetadata>>(async dependencyName => {
        let dependencyDirectory = path.resolve(dependencyPath, getDependencyName(dependencyName));
        if (isDebug) dependencyDirectory += "-debug";

        const metaFilePath = path.resolve(dependencyDirectory, "./meta.json");
        const meta = JSON.parse((await fs.readFile(metaFilePath)).toString());

        meta.includePath = compileDependencyMetadata(meta.includePath, dependencyDirectory);
        meta.libraryPath = compileDependencyMetadata(meta.libraryPath, dependencyDirectory);

        return meta;
    });

    const metadataArray = await Promise.all(metadataPromises);
    const metadata: DependencyMetadata = {
        includePath: flatten(metadataArray.map(m => m.includePath)),
        libraryPath: flatten(metadataArray.map(m => m.libraryPath)),
    };

    metadata.includePath.push(`<!@(node -p "require('node-addon-api').include")`);

    let binding = (await fs.readFile(bindingTemplatePath)).toString();
    binding = binding.replace(/"%INCLUDE_PATH%"/g, JSON.stringify(metadata.includePath));

    const libraryPath: string[] = metadata.libraryPath as any;
    const libraryFiles: string[] = libraryPath
        .map(itemPath => (fs.lstatSync(itemPath).isFile() ? itemPath : null))
        .filter(itemPath => itemPath);

    binding = binding.replace(/"%LIBRARY_FILES%"/g, JSON.stringify(libraryFiles));

    const libraryDirectories: string[] = libraryPath
        .map(itemPath => (fs.lstatSync(itemPath).isDirectory() ? itemPath : null))
        .filter(itemPath => itemPath);

    const GCCLibraryDirectories: string[] = libraryDirectories.map(directory => `-L${directory}`);
    const MSVCLibraryDirectories: string[] = libraryDirectories;

    binding = binding.replace(/"%GCC_LIBRARY_PATH%"/g, JSON.stringify(GCCLibraryDirectories));
    binding = binding.replace(/"%MSVC_LIBRARY_PATH%"/g, JSON.stringify(MSVCLibraryDirectories));

    await fs.writeFile(bindingPath, binding);
}
