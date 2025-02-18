import {expectType} from 'tsd'
import * as clip from '..'
const {
  clear,
  getChangeCount,
  hasBufferForFormat,
  readBuffer,
  readText,
  writeBuffer,
  writeText,
  FORMAT_PLAIN_TEXT,
  FORMAT_FILE_URL,
} = clip

expectType<void>(clip.clear())

expectType<Number>(clip.getChangeCount())
expectType<boolean>(clip.hasBufferForFormat())

expectType<Buffer>(readBuffer(FORMAT_PLAIN_TEXT))
expectType<string>(readText(FORMAT_PLAIN_TEXT))

expectType<boolean>(writeBuffer(FORMAT_PLAIN_TEXT, Buffer.from(__filename)))
expectType<boolean>(writeText(FORMAT_PLAIN_TEXT, __filename))
