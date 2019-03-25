import { Buffer } from "nbind/dist/shim";

export class NBindBase { free?(): void }

export class Greeter extends NBindBase {
	/** static void sayHello(std::string); */
	static sayHello(p0: string): void;
}
