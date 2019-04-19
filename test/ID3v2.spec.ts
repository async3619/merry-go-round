import * as path from "path";
import * as fs from "fs";
import { assert, expect } from "chai";

import * as merryGoRound from "../src";
import { ID3v2ChapterFrame } from "../src/types";

const SAMPLES_PATH = path.resolve(__dirname, "./samples");
const getSamplePath = (fileName: string) => path.resolve(SAMPLES_PATH, fileName);
const needSnapshot = process.argv.indexOf("--snapshot") >= 0;

function readNativeData(filePath: string) {
    const music = merryGoRound.loadFromFile(getSamplePath(filePath));
    return music.nativeData();
}

describe("Music::ID3v2", () => {
    it("should be able to read album cover image data of media file (ID3v2::APIC)", () => {
        const nativeData = readNativeData("auphonic_chapters_demo.mp3");

        expect(nativeData.dataType).to.equal("ID3v2");
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

    it("should be able to read full information of comment data of media file (ID3v2::COMM)", () => {
        const nativeData = readNativeData("RushJet1 - Forgotten Music - 02 - Ancient Ruin Adventure.mp3");
        const snapshotPath = getSamplePath("RushJet1 - Forgotten Music - 02 - Ancient Ruin Adventure.comments.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.COMM && Array.isArray(nativeData.COMM));

            if (Array.isArray(nativeData.COMM)) {
                if (needSnapshot) {
                    fs.writeFileSync(snapshotPath, JSON.stringify(nativeData.COMM));
                } else {
                    const snapshotData = JSON.parse(fs.readFileSync(snapshotPath).toString());
                    assert.deepEqual(snapshotData, nativeData.COMM);
                }
            }
        }
    });

    it("should be able to read private information data of media file (ID3v2::PRIV)", () => {
        const nativeData = readNativeData("Witness-06-Farewell.mp3");
        const snapshotPath = getSamplePath("Witness-06-Farewell.private.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.PRIV);
            assert(Array.isArray(nativeData.PRIV)); // PRIV frame should be multiple for this file.

            if (Array.isArray(nativeData.PRIV)) {
                const result = nativeData.PRIV.map(privateFrame => ({
                    owner: privateFrame.owner,
                    data: privateFrame.data.toString(),
                }));

                if (needSnapshot) {
                    fs.writeFileSync(snapshotPath, JSON.stringify(result));
                } else {
                    const snapshotData = JSON.parse(fs.readFileSync(snapshotPath).toString());
                    assert.deepEqual(snapshotData, result);
                }
            }
        }
    });

    it("should provide all chapter information of media file (ID3v2::CHAP)", () => {
        const nativeData = readNativeData("auphonic_chapters_demo.mp3");
        const snapshotPath = getSamplePath("auphonic_chapters_demo.chapters.json");

        expect(nativeData.dataType).to.equal("ID3v2");
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

                const data = JSON.stringify(dataArray);

                if (needSnapshot) {
                    fs.writeFileSync(snapshotPath, data);
                } else {
                    const snapshotData = JSON.parse(fs.readFileSync(snapshotPath).toString());
                    assert.deepEqual(snapshotData, dataArray);
                }
            }
        }
    });
});
