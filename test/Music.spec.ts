import * as path from "path";
import { assert, expect } from "chai";

import { Music } from "../src";

const SAMPLES_PATH = path.resolve(__dirname, "./samples");

describe("Music class", () => {
    it("should load music file through `create` method", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));

        assert(music);
    });

    it("should parse tag information from music file", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
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

    it("should be able to get music file type", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        expect(music.type()).to.equal("MPEG");
    });

    it("should be able to get music tag type", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        expect(music.tagType()).to.equal("ID3v2.3");
    });
});
