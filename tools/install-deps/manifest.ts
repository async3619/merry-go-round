import * as fs from "fs-extra";
import * as path from "path";

import { dependencyPath, ObjectType } from "./utils";

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
    const manifest = await readManifest();
    manifest[dependencyName] = versionName.trim();

    await fs.writeFile(dependencyManifestPath, JSON.stringify(manifest, null, 4));
}
