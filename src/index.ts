import * as nbind from "nbind";
import * as LibTypes from "./lib-types";

const { lib } = nbind.init<typeof LibTypes>();

export const { Music } = lib;
