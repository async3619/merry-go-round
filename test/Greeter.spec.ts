import { Greeter } from "../src";
import { expect } from "chai";

describe("Greeter class", () => {
    it("should return true", () => {
        expect(Greeter.sayHello("Test")).to.equal(true);
    });
});
