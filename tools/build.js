const spawn = require("cross-spawn");
const npmRunPath = require("npm-run-path");
const fs = require("fs");
const path = require("path");
const rimraf = require("rimraf");

const isDebug = (process.env.MERRY_GO_ROUND_BUILD_MODE || "").toLowerCase() === "debug";
const BUILD_MODE = isDebug ? "Debug" : "Release";
const BUILD_PATH = path.resolve(__dirname, "..", "build");
const BUILD_ENV_CACHE = path.resolve(BUILD_PATH, ".env");

const runtime = process.env.npm_config_runtime || "node";
const target = process.env.npm_config_target || process.versions.node;
const abi = process.env.npm_config_abi || process.versions.modules;

const promisedRimraf = (targetPath) => {
    return new Promise((resolve) => {
        rimraf(targetPath, {}, () => {
            resolve();
        });
    });
};

const deleteBuildPath = async () => {
    console.log("Deleting './build' ...");
    await promisedRimraf(BUILD_PATH);
};

(async () => {
    if (fs.existsSync(BUILD_PATH)) {
        if (fs.existsSync(BUILD_ENV_CACHE)) {
            const lastEnv = fs.readFileSync(BUILD_ENV_CACHE).toString();
            if (lastEnv !== BUILD_MODE) {
                await deleteBuildPath();
            }
        } else {
            await deleteBuildPath();
        }
    }

    console.log('BUILD for %s@%s (abi=%s) %s', runtime, target, abi, isDebug ? "[DEBUG]" : "");

    const args = [
        isDebug ? "build" : "rebuild",
        "-r", runtime,
        "-v", target,
        "--abi", abi,
    ].concat(isDebug ? ["--debug"] : []);

    const ps = spawn("cmake-js", args, {
        env: npmRunPath.env(),
    });

    ps.stdout.pipe(process.stdout);
    ps.stderr.pipe(process.stderr);

    ps.on('exit', (code) => {
        if (code === 0 && fs.existsSync(BUILD_PATH)) {
            fs.writeFileSync(path.resolve(__dirname, "..", "build", ".env"), BUILD_MODE);
        }

        // eslint-disable-next-line unicorn/no-process-exit
        process.exit(code);
    });

})();