export type Arrayable<T> = T | T[];
export type Omit<T, K> = Pick<T, Exclude<keyof T, K>>

export interface Range {
    start: number;
    end: number;
}

export interface ID3v2ChapterFrame {
    time: Range;
    offset: Range;
    embedded?: Omit<ID3v2NativeData, "dataType">;
}

export interface ID3v2AttachedPictureFrame {
    mimeType: string;
    type: { id: number; description: string };
    data: Buffer;
    description: string;
}

export interface ID3v2PrivateFrame {
    owner: string;
    data: Buffer;
}

export interface UnknownNativeData {
    dataType: "Unknown";
}

export interface ID3v2NativeData {
    dataType: "ID3v2";
    AENC?: string;
    APIC?: Arrayable<ID3v2AttachedPictureFrame>;
    CHAP?: Arrayable<ID3v2ChapterFrame>;
    COMM?: string;
    COMR?: string;
    ENCR?: string;
    EQUA?: string;
    ETCO?: string;
    GEOB?: string;
    GRID?: string;
    IPLS?: string;
    LINK?: string;
    MCDI?: string;
    MLLT?: string;
    OWNE?: string;
    PCNT?: string;
    POPM?: string;
    POSS?: string;
    PRIV?: Arrayable<ID3v2PrivateFrame>;
    RBUF?: string;
    RVAD?: string;
    RVRB?: string;
    SYLT?: string;
    SYTC?: string;
    TALB?: string;
    TBPM?: string;
    TCOM?: string;
    TCON?: string;
    TCOP?: string;
    TDAT?: string;
    TDLY?: string;
    TENC?: string;
    TEXT?: string;
    TFLT?: string;
    TIME?: string;
    TIT1?: string;
    TIT2?: string;
    TIT3?: string;
    TKEY?: string;
    TLAN?: string;
    TLEN?: string;
    TMED?: string;
    TOAL?: string;
    TOFN?: string;
    TOLY?: string;
    TOPE?: string;
    TORY?: string;
    TOWN?: string;
    TPE1?: string;
    TPE2?: string;
    TPE3?: string;
    TPE4?: string;
    TPOS?: string;
    TPUB?: string;
    TRCK?: string;
    TRDA?: string;
    TRSN?: string;
    TRSO?: string;
    TSIZ?: string;
    TSRC?: string;
    TSSE?: string;
    TXXX?: string;
    TYER?: string;
    UFID?: string;
    USER?: string;
    USLT?: string;
    WCOM?: string;
    WCOP?: string;
    WOAF?: string;
    WOAR?: string;
    WOAS?: string;
    WORS?: string;
    WPAY?: string;
    WPUB?: string;
    WXXX?: string;
}

export interface NativeMusic {
    title(): string;
    artist(): string;
    album(): string;
    genre(): string;
    year(): string;
    track(): string;
    tagType(): string;
    nativeData(): UnknownNativeData | ID3v2NativeData;
}

export interface NativeAddon {
    loadFromFile(path: String): NativeMusic;
}

export interface MerryGoRound extends NativeAddon {}
