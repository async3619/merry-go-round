import * as path from "path";
import * as fs from "fs";
import { assert, expect } from "chai";

import * as merryGoRound from "../src";
import { ID3v2ChapterFrame, NativeMusic } from "../src/types";

type Promisable<T> = T | Promise<T>;

const SAMPLES_PATH = path.resolve(__dirname, "./samples");
const getSamplePath = (fileName: string) => path.resolve(SAMPLES_PATH, fileName);
const needSnapshot = process.argv.indexOf("--snapshot") >= 0;

function readNativeData(filePath: string) {
    const music = merryGoRound.loadFromFile(getSamplePath(filePath));
    return music.nativeData();
}

async function runSnapshotTest(fileName: string, worker: (music: NativeMusic) => Promisable<any>) {
    const samplePath = getSamplePath(fileName);
    const music = merryGoRound.loadFromFile(samplePath);
    const data = await worker(music);
    const parsedPath = path.parse(samplePath);
    const snapshotFileName = path.join(
        parsedPath.dir,
        "__snapshot__",
        path.parse(fileName).name + ".__snapshot__.json",
    );

    if (needSnapshot) {
        fs.writeFileSync(snapshotFileName, JSON.stringify(data));
    } else {
        const rawJSON = fs.readFileSync(snapshotFileName).toString();
        const parsedData = JSON.parse(rawJSON);

        assert.deepEqual(data, parsedData);
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

    it("should provide all chapter information of media file (ID3v2::CHAP)", async () => {
        await runSnapshotTest("auphonic_chapters_demo.mp3", music => {
            const nativeData = music.nativeData();

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

                    return dataArray;
                }
            }

            return null;
        });
    });

    it("should read full information of comment data of media file (ID3v2::COMM)", async () => {
        await runSnapshotTest("RushJet1 - Forgotten Music - 02 - Ancient Ruin Adventure.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                assert(nativeData.COMM && Array.isArray(nativeData.COMM));

                if (Array.isArray(nativeData.COMM)) {
                    return nativeData.COMM;
                }
            }

            return null;
        });
    });

    it("should parse all event timing code data of media file (ID3v2::ETCO)", async () => {
        await runSnapshotTest("id3v2.etco.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                assert(nativeData.ETCO);
                if (nativeData.ETCO) {
                    return nativeData.ETCO;
                }
            }

            return null;
        });
    });

    it("should parse ownership information of media file (ID3v2::OWNE)", async () => {
        await runSnapshotTest("id3v2.owne.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                // There may only be one "OWNE" frame in a tag.
                assert(nativeData.OWNE && !Array.isArray(nativeData.OWNE));
                if (nativeData.OWNE) {
                    return nativeData.OWNE;
                }
            }

            return null;
        });
    });

    it("should parse popularimeter information of media file (ID3v2::POPM)", async () => {
        await runSnapshotTest("id3v2.popm.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                // There may be more than one "POPM" frame in each tag
                assert(nativeData.POPM && Array.isArray(nativeData.POPM));
                if (nativeData.POPM && Array.isArray(nativeData.POPM)) {
                    return nativeData.POPM;
                }
            }

            return null;
        });
    });

    it("should read private information data of media file (ID3v2::PRIV)", async () => {
        await runSnapshotTest("Witness-06-Farewell.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                assert(nativeData.PRIV);
                assert(Array.isArray(nativeData.PRIV)); // PRIV frame should be multiple for this file.

                if (Array.isArray(nativeData.PRIV)) {
                    return nativeData.PRIV.map(privateFrame => ({
                        owner: privateFrame.owner,
                        data: privateFrame.data.toString(),
                    }));
                }
            }

            return null;
        });
    });

    it("should read relative volume settings of media file (ID3v2::RVA2)", async () => {
        await runSnapshotTest("id3v2.rva2.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                // There may be more than one "RVA2" frame in each tag
                assert(nativeData.RVA2 && Array.isArray(nativeData.RVA2));
                if (nativeData.RVA2 && Array.isArray(nativeData.RVA2)) {
                    return nativeData.RVA2;
                }
            }

            return null;
        });
    });

    it("should read all synchronised lyrics data of media file (ID3v2::SYLT)", async () => {
        await runSnapshotTest("id3v2.sylt.mp3", music => {
            const nativeData = music.nativeData();

            expect(nativeData.dataType).to.equal("ID3v2");
            if (nativeData.dataType === "ID3v2") {
                // There may be more than one "SYLT" frame in each tag
                assert(nativeData.SYLT && Array.isArray(nativeData.SYLT));
                if (nativeData.SYLT && Array.isArray(nativeData.SYLT)) {
                    return nativeData.SYLT;
                }
            }

            return null;
        });
    });
});
