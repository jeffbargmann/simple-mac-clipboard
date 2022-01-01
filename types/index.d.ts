// alias
export function clear(): void
export function getChangeCount(): Number
export function hasBufferForFormat(format: string): boolean

// buffer
export function readBuffer(format: string): Buffer
export function writeBuffer(format: string, data: Buffer): boolean

// text
export function readText(format: string): string
export function writeText(format: string, text: string): boolean

// format
export const FORMAT_PLAIN_TEXT: string
export const FORMAT_FILE_URL: string
