import fetch from "node-fetch";
import * as fs from "fs-extra";
import * as Url from "url";
import * as path from "path";

import { readManifest, writeManifest } from "./manifest";
import { dependencyPath, getArtifactName, pathDictionary } from "./utils";

async function downloadFile(url: string, targetPath: string) {
    const res = await fetch(url);

    await new Promise((resolve, reject) => {
        const fileStream = fs.createWriteStream(targetPath);
        res.body.pipe(fileStream);
        res.body.on("error", err => {
            reject(err);
        });
        fileStream.on("finish", () => {
            resolve();
        });
    });
}
export async function downloadDependency(dependencyName: string) {
    const [user, repo, branch] = dependencyName.split("/");
    const latestVersionUrl = `https://raw.githubusercontent.com/${user}/${repo}/${branch || "master"}/.latest_version`;

    const response = await fetch(latestVersionUrl);
    if (!response.ok) {
        throw new Error(`given dependency '${dependencyName}' don't seem valid native dependency.`);
    }

    const versionName = (await response.text()).trim();
    const manifest = await readManifest();
    if (manifest[versionName] === versionName) {
        console.log(`dependency '${dependencyName}' seems already installed up-to-date. skip it.`);
        return;
    }

    await writeManifest(dependencyName, versionName);

    const desiredArtifactName = getArtifactName();
    const artifactUrl = `https://github.com/${user}/${repo}/releases/download/${versionName}/${branch}-${desiredArtifactName}.tar.gz`;
    const url = Url.parse(artifactUrl);
    const filename = path.basename(url.pathname);

    pathDictionary[dependencyName] = path.resolve(dependencyPath, filename);

    await downloadFile(artifactUrl, pathDictionary[dependencyName]);
}
