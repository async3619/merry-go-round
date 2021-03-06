export type Arrayable<T> = T | T[];
export type Omit<T, K> = Pick<T, Exclude<keyof T, K>>;
export type SupportedFileType =
    | "MPEG"
    | "Ogg::Vorbis"
    | "Ogg::FLAC"
    | "FLAC"
    | "MPC"
    | "WavPack"
    | "Ogg::Speex"
    | "Ogg::Opus"
    | "TrueAudio"
    | "MP4"
    | "ASF"
    | "RIFF::AIFF"
    | "RIFF::WAV"
    | "APE"
    | "DSDIFF"
    | "DSF";

export interface Range {
    start: number;
    end: number;
}

export interface TypeData {
    id: number;
    description: string;
}

export interface ID3v2AttachedPictureFrame {
    mimeType: string;
    type: TypeData;
    data: Buffer;
    description: string;
}

export interface ID3v2ChapterFrame {
    time: Range;
    offset: Range;
    embedded?: Omit<ID3v2NativeData, "dataType">;
}

export interface ID3v2CommentsFrame {
    language: string;
    text: string;
    description: string;
}

export interface ID3v2Event {
    type: TypeData;
    time: number;
}

export interface ID3v2EventTimeCodesFrame {
    format: TypeData;
    events: ID3v2Event[];
}

export interface ID3v2OwnershipFrame {
    // The price paid.
    pricePaid: number;

    // The date purchased.
    datePurchased: string;

    // The seller.
    seller: string;
}

export interface ID3v2PrivateFrame {
    owner: string;
    data: Buffer;
}

export interface ID3v2PeakVolume {
    // The number of bits (in the range of 0 to 255) used to describe the peak volume.
    bitsRepresenting: number;

    // The array of bits (represented as a series of bytes) used to describe the peak volume.
    volume: Buffer;
}

export interface ID3v2PopularimeterFrame {
    // The email address.
    email: string;

    // The rating value.
    rating: string;

    // The counter value.
    counter: string;
}

export interface ID3v2RelativeVolumeChannel {
    // A type information of channel.
    type: TypeData;

    // The relative volume adjustment in decibels.
    adjustment: number;

    // The relative volume adjustment "index". As indicated by the ID3v2 standard this is a 16-bit signed integer that
    // reflects the decibels of adjustment when divided by 512.
    adjustmentIndex: number;

    // The peak volume (represented as a length and a string of bits).
    // peak: ID3v2PeakVolume;
}

export interface ID3v2RelativeVolumeFrame {
    // The identification for this frame
    identification: string;

    // A list of channels with information currently in the frame.
    channels: ID3v2RelativeVolumeChannel[];
}

interface ID3v2SynchedText {
    text: string;
    time: number;
}

export interface ID3v2SynchronizedLyricsFrame {
    // The language encoding as a 3 byte encoding as specified by ISO-639-2.
    language: string;

    // The timestamp format.
    format: TypeData;

    // The type of text contained.
    type: TypeData;

    // The description of this synchronized lyrics frame.
    description: string;

    // The text with the time stamps.
    synchedText: ID3v2SynchedText[];
}

export interface UnknownNativeData {
    dataType: "Unknown";
}

export interface ID3v2NativeData {
    dataType: "ID3v2";
    AENC?: string;
    APIC?: Arrayable<ID3v2AttachedPictureFrame>;
    CHAP?: Arrayable<ID3v2ChapterFrame>;
    COMM?: Arrayable<ID3v2CommentsFrame>;
    COMR?: string;
    ENCR?: string;
    EQUA?: string;
    ETCO?: ID3v2EventTimeCodesFrame;
    GEOB?: string;
    GRID?: string;
    IPLS?: string;
    LINK?: string;
    MCDI?: string;
    MLLT?: string;
    OWNE?: ID3v2OwnershipFrame;
    PCNT?: string;
    POPM?: Arrayable<ID3v2PopularimeterFrame>;
    POSS?: string;
    PRIV?: Arrayable<ID3v2PrivateFrame>;
    RBUF?: string;
    RVAD?: string;
    RVA2?: Arrayable<ID3v2RelativeVolumeFrame>;
    RVRB?: string;
    SYLT?: Arrayable<ID3v2SynchronizedLyricsFrame>;
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
    fileType(): SupportedFileType;
}

export interface NativeAddon {
    loadFromFile(path: String): NativeMusic;
}

export interface MerryGoRound extends NativeAddon {}
