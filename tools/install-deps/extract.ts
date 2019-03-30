import * as path from "path";
import * as fs from "fs-extra";
import * as tar from "tar";

import { dependencyPath, getDependencyName, pathDictionary } from "./utils";

export async function extractDependency(dependencyName: string) {
    const targetDirectory = path.resolve(dependencyPath, getDependencyName(dependencyName));
    await fs.ensureDir(targetDirectory);

    await tar.x({
        cwd: targetDirectory,
        file: pathDictionary[dependencyName],
        stripComponents: 1,
    });
}
