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

function snapshot(snapshotPath: string, expected: any) {
    if (needSnapshot) {
        fs.writeFileSync(snapshotPath, JSON.stringify(expected));
    } else {
        const snapshotData = JSON.parse(fs.readFileSync(snapshotPath).toString());
        assert.deepEqual(snapshotData, expected);
    }
}

describe("Music::ID3v2", () => {
    it("should read album cover image data of media file (ID3v2::APIC)", () => {
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

                snapshot(snapshotPath, dataArray);
            }
        }
    });

    it("should read full information of comment data of media file (ID3v2::COMM)", () => {
        const nativeData = readNativeData("RushJet1 - Forgotten Music - 02 - Ancient Ruin Adventure.mp3");
        const snapshotPath = getSamplePath("RushJet1 - Forgotten Music - 02 - Ancient Ruin Adventure.comments.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.COMM && Array.isArray(nativeData.COMM));

            if (Array.isArray(nativeData.COMM)) {
                snapshot(snapshotPath, nativeData.COMM);
            }
        }
    });

    it("should parse all event timing code data of media file (ID3v2::ETCO)", () => {
        const nativeData = readNativeData("id3v2.etco.mp3");
        const snapshotPath = getSamplePath("id3v2.etco.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.ETCO);
            if (nativeData.ETCO) {
                snapshot(snapshotPath, nativeData.ETCO);
            }
        }
    });

    it("should parse ownership information of media file (ID3v2::OWNE)", () => {
        const nativeData = readNativeData("id3v2.owne.mp3");
        const snapshotPath = getSamplePath("id3v2.owne.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            assert(nativeData.OWNE && !Array.isArray(nativeData.OWNE)); // There may only be one "OWNE" frame in a tag.
            if (nativeData.OWNE) {
                snapshot(snapshotPath, nativeData.OWNE);
            }
        }
    });

    it("should parse popularimeter information of media file (ID3v2::POPM)", () => {
        const nativeData = readNativeData("id3v2.popm.mp3");
        const snapshotPath = getSamplePath("id3v2.popm.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            // There may be more than one "POPM" frame in each tag
            assert(nativeData.POPM && Array.isArray(nativeData.POPM));
            if (nativeData.POPM && Array.isArray(nativeData.POPM)) {
                snapshot(snapshotPath, nativeData.POPM);
            }
        }
    });

    it("should read private information data of media file (ID3v2::PRIV)", () => {
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

                snapshot(snapshotPath, result);
            }
        }
    });

    it("should read relative volume settings of media file (ID3v2::RVA2)", () => {
        const nativeData = readNativeData("id3v2.rva2.mp3");
        const snapshotPath = getSamplePath("id3v2.rva2.json");

        expect(nativeData.dataType).to.equal("ID3v2");
        if (nativeData.dataType === "ID3v2") {
            // There may be more than one "RVA2" frame in each tag
            assert(nativeData.RVA2 && Array.isArray(nativeData.RVA2));
            if (nativeData.RVA2 && Array.isArray(nativeData.RVA2)) {
                snapshot(snapshotPath, nativeData.RVA2);
            }
        }
    });
});
