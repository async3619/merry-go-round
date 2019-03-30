import * as fs from "fs-extra";
import * as Listr from "listr";

import { downloadDependency } from "./download";
import { extractDependency } from "./extract";
import { installDependency } from "./install";

import { dependencies, dependencyPath } from "./utils";

fs.ensureDirSync(dependencyPath);

const tasks = new Listr([
    {
        title: "Download dependency artifacts",
        task: () => {
            return new Listr(
                dependencies.map<Listr.ListrTask>(dependency => {
                    return {
                        title: `Downloading ${dependency}`,
                        task: () => downloadDependency(dependency),
                    };
                }),
            );
        },
    },
    {
        title: "Extract dependency artifacts",
        task: () => {
            return new Listr(
                dependencies.map<Listr.ListrTask>(dependency => {
                    return {
                        title: `Extracting ${dependency}`,
                        task: () => extractDependency(dependency),
                    };
                }),
            );
        },
    },
    {
        title: "Install native dependencies",
        task: () => {
            return new Listr([
                {
                    title: `Installing dependencies`,
                    task: () => installDependency(dependencies),
                },
            ]);
        },
    },
]);

tasks
    .run()
    .then()
    .catch(error => {
        console.error(`Error: ${error.message}`);
    });
