/* eslint-disable global-require */
import { NativeAddon } from "./types";

const merryGoRound: NativeAddon = require("bindings")("merry-go-round");

export const { loadFromFile } = merryGoRound;
