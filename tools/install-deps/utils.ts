import * as path from "path";
import filenamify from "filenamify";

import * as packageInformation from "../../package.json";

const { nativeDependencies } = packageInformation;

export interface ObjectType<T> {
    [key: string]: T;
}

export type DependencyMetadataItem = string | [string, string] | [string, string, string];

export interface DependencyMetadata {
    libraryPath: DependencyMetadataItem[];
    includePath: DependencyMetadataItem[];
}

export const isDebug = process.env.NODE_ENV === "debug";
export const rootPath = path.resolve(__dirname, "../..");
export const bindingTemplatePath = path.resolve(rootPath, "./binding.template.gyp");
export const bindingPath = path.resolve(rootPath, "./binding.gyp");
export const dependencyPath = path.resolve(rootPath, "deps");
export const dependencies = nativeDependencies.map(dependencyName => {
    return dependencyName + (isDebug ? "/debug" : "");
});
export const pathDictionary: ObjectType<string> = {};

export function getArtifactName(): string {
    let platform: string = process.platform;
    if (platform === "win32") platform = "windows";
    else if (platform === "darwin") platform = "osx";

    return `${platform}-${process.arch}-static` + (isDebug ? "-debug" : "");
}

export function filenamifyDependencyName(dependencyName: string): string {
    return filenamify(dependencyName, { replacement: "_" });
}

export function getDependencyPath(dependencyName: string) {
    const directoryName = getDependencyName(dependencyName) + (isDebug ? "-debug" : "");
    return path.resolve(dependencyPath, directoryName);
}

export function getDependencyName(rawName: string): string {
    const tokens = rawName.split("/");
    return tokens[2] || tokens[1];
}
