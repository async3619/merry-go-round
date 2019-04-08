import * as path from "path";
import { assert, expect } from "chai";

import { Music, BufferManager } from "../src";
import * as fs from "fs";

const SAMPLES_PATH = path.resolve(__dirname, "./samples");

describe("Music class", () => {
    it("should load media file through `create` method", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));

        assert(music);
    });

    it("should parse tag information from media file", () => {
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

    it("should be able to get media file type", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        expect(music.type()).to.equal("MPEG");
    });

    it("should be able to get media tag type", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        expect(music.tagType()).to.equal("ID3v2.3");
    });

    it("should be able to get raw tag data of media file", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        assert(music.nativeData());
    });

    it("should be able to read album cover image data of media file", () => {
        const music = Music.create(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.mp3"));
        const musicData = music.nativeData();
        const sampleAlbumCoverData = fs.readFileSync(path.resolve(SAMPLES_PATH, "Witness-06-Farewell.cover.jpeg"));

        musicData.forEach(([key, value, ...rest]) => {
            if (key === "APIC") {
                const [mimeType, imageType, description, bufferKey, bufferLengthString] = [value, ...rest];
                const bufferLength = parseInt(bufferLengthString, 10);
                const imageBuffer = Buffer.alloc(bufferLength, 0);

                BufferManager.getInstance().readReservedBuffer(bufferKey, imageBuffer);

                expect([value, ...rest].length).to.equal(5);
                expect(mimeType).to.equal("image/jpeg");
                expect(imageType).to.equal("Cover (front)");
                expect(description).to.equal("");
                assert(Buffer.compare(imageBuffer, sampleAlbumCoverData) === 0);
            }
        });
    });
});
