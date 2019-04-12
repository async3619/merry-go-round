import * as glob from "glob";
import * as path from "path";

const PROJECT_ROOT = path.resolve(__dirname, "..", "..");
const SOURCE_ROOT = path.resolve(PROJECT_ROOT, "native");

glob(path.resolve(SOURCE_ROOT, "**/*.cc"), {}, (err, matches) => {
    if (err) {
        throw err;
    }

    matches.forEach(filePath => console.log(filePath));
});
