const spawn = require("cross-spawn");
const npmRunPath = require("npm-run-path");

const isDebug = (process.env.NODE_ENV || "").toLowerCase() === "debug";

const runtime = process.env.npm_config_runtime || "node";
const target = process.env.npm_config_target || process.versions.node;
const abi = process.env.npm_config_abi || process.versions.modules;

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
    // eslint-disable-next-line unicorn/no-process-exit
    process.exit(code)
});
