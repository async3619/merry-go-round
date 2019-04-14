import * as path from "path";
import * as fs from "fs";
import { assert, expect } from "chai";

import * as merryGoRound from "../src";
import { ID3v2ChapterFrame } from "../src/types";

const SAMPLES_PATH = path.resolve(__dirname, "./samples");
const needSnapshot = process.argv.indexOf("--snapshot") >= 0;

describe("Music::ID3v2", () => {
    it("should be able to read album cover image data of media file (ID3v2::APIC)", () => {
        const music = merryGoRound.loadFromFile(path.resolve(SAMPLES_PATH, "auphonic_chapters_demo.mp3"));
        const nativeData = music.nativeData();
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.APIC);
            if (!Array.isArray(nativeData.APIC)) {
                expect(nativeData.APIC.type.id).to.be.equal(0x03);
                expect(nativeData.APIC.type.description).to.be.equal("Cover (front)");
                expect(nativeData.APIC.description).to.be.equal("");
                expect(nativeData.APIC.mimeType).to.be.equal("image/jpeg");
                assert(nativeData.APIC.data instanceof Buffer);

                const buffer = fs.readFileSync(path.resolve(SAMPLES_PATH, "./auphonic_chapters_demo.cover.jpeg"));
                assert(buffer.equals(nativeData.APIC.data));
            }
        }
    });

    it("should provide all chapter informations of media file (ID3v2::CHAP)", () => {
        const music = merryGoRound.loadFromFile(path.resolve(SAMPLES_PATH, "auphonic_chapters_demo.mp3"));
        const nativeData = music.nativeData();
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.CHAP);
            assert(Array.isArray(nativeData.CHAP));

            if (Array.isArray(nativeData.CHAP)) {
                const dataArray: ID3v2ChapterFrame[] = [];
                nativeData.CHAP.forEach(chapter => {
                    assert(chapter.time);
                    assert(chapter.offset);

                    if (chapter.embedded.APIC) {
                        if (!Array.isArray(chapter.embedded.APIC)) {
                            delete chapter.embedded.APIC.data;
                        } else {
                            chapter.embedded.APIC = chapter.embedded.APIC.map(picture => {
                                delete picture.data;
                                return picture;
                            });
                        }
                    }

                    dataArray.push({
                        time: chapter.time,
                        offset: chapter.offset,
                        embedded: chapter.embedded,
                    });
                });

                if (needSnapshot) {
                    fs.writeFileSync(
                        path.resolve(SAMPLES_PATH, "./auphonic_chapters_demo.chapters.json"),
                        JSON.stringify(dataArray, null, 4),
                    );
                } else {
                    const snapshotData = fs
                        .readFileSync(path.resolve(SAMPLES_PATH, "./auphonic_chapters_demo.chapters.json"))
                        .toString();

                    expect(JSON.stringify(dataArray, null, 4)).to.be.equal(snapshotData);
                }
            }
        }
    });
});
