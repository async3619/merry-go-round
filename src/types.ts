export interface NativeMusic {
    title(): string;
    artist(): string;
    album(): string;
    genre(): string;
    year(): string;
    track(): string;
}

export interface NativeAddon {
    loadFromFile(path: String): NativeMusic;
}

export interface MerryGoRound extends NativeAddon {}
