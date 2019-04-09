import * as fs from "fs-extra";
import * as tar from "tar";

import { getDependencyPath, pathDictionary } from "./utils";

export async function extractDependency(dependencyName: string) {
    if (!pathDictionary[dependencyName]) return;

    const targetDirectory = getDependencyPath(dependencyName);
    await fs.ensureDir(targetDirectory);

    await tar.x({
        cwd: targetDirectory,
        "keep-newer-files": true,
        file: pathDictionary[dependencyName],
        stripComponents: 1,
    });
}
