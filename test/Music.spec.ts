import * as path from "path";
import { assert, expect } from "chai";

import * as merryGoRound from "../src";
import { NativeMusic } from "../src/types";

const SAMPLES_PATH = path.resolve(__dirname, "./samples");
let music: NativeMusic | null = null;

describe("Music", () => {
    it("should load media file through `loadFromFile` method", () => {
        music = merryGoRound.loadFromFile(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        assert(music);
    });

    it("should parse tag information from media file", () => {
        const tagInformation = {
            title: music.title(),
            artist: music.artist(),
            album: music.album(),
            genre: music.genre(),
            track: music.track(),
            year: music.year(),
        };

        expect(tagInformation).to.deep.equal({
            title: "Farewell",
            artist: "Witness",
            album: "Hope Springs Eternal: A Tribute To Nujabes",
            genre: "Rap / Hip-hop",
            track: 6,
            year: 2010,
        });
    });

    it("should be able to get media file type", () => {
        expect(music.fileType()).to.equal("MPEG");
    });

    it("should be able to get media tag type", () => {
        expect(music.tagType()).to.equal("ID3v2.3");
    });

    it("should be able to get raw tag data of media file", () => {
        assert(music.nativeData());
    });
});
