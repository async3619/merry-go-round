import { Music } from "../src";
import { assert } from "chai";

describe("Music class", () => {
    it("should have static `create` method", () => {
        assert(Music.create);
    });
});
