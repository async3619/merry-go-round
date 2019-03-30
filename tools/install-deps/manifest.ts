import * as fs from "fs-extra";
import * as path from "path";

import { dependencies, dependencyPath, ObjectType } from "./utils";

const dependencyManifestPath = path.resolve(dependencyPath, "./.manifest.json");

export async function readManifest(): Promise<ObjectType<string>> {
    let manifest: ObjectType<string> = {};
    if (fs.existsSync(dependencyManifestPath)) {
        const data = await fs.readFile(dependencyManifestPath);
        manifest = JSON.parse(data.toString());
    }

    return manifest;
}

export async function writeManifest(dependencyName: string, versionName: string) {
    const manifest = renewManifest(await readManifest());
    manifest[dependencyName] = versionName.trim();

    await fs.writeFile(dependencyManifestPath, JSON.stringify(manifest, null, 4));
}

export function renewManifest(manifest: ObjectType<string>): ObjectType<string> {
    const result = { ...manifest };
    Object.keys(result).forEach(dependencyName => {
        if (dependencies.indexOf(dependencyName) === -1) {
            delete result[dependencyName];
        }
    });

    return result;
}
