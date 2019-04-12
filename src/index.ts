/* eslint-disable global-require */
import { MerryGoRound, NativeAddon } from "./types";

const merryGoRound: NativeAddon = require("bindings")("merry-go-round");

export const { loadFromFile }: MerryGoRound = merryGoRound;
