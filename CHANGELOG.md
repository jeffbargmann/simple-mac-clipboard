# CHANGELOG

## v1.1.0 2021-09-16

- add getChangeCount for pasteboard change detection
- remove ambiguity from description; library works with both Electron and vanilla Node.js

## v1.0.1 2021-01-21

- update prebuild-install to latest, in order to support electron@10

## v1.0.0 2020-07-27

- use N-API

## v0.3.2 2020-03-02

- fix webpack usage

## v0.3.1 2020-03-02

- use jsdelivr as binary host

## v0.3.0 2020-03-01

- use `[NSData dataWithBytes]` insteadof `[NSData dataWithBytesNoCopy]` in `writeBuffer`,
  the no-copy one make when `paste in electron apps` freeze

## v0.2.1 2020-03-01

- I forget run `yarn gen-readme`, and I quit `npm-minor` process.
- so it's same to v0.2.0

## v0.2.0 2020-03-01

- add support for `read` clipboard, add support for write Buffer data
- refactor function names to `clear` & `readBuffer/writeBuffer` & `readText/writeText`

## v0.1.1 2020-02-27

- add return value for `setStringData`

## v0.1.0 2020-02-27

- add parameter check, addon will throw TypeError when it's not right.
- get rid of `electron-rebuild`, sometimes it does not rebuild. That's annoying.
- add types/index.d.ts file

## v0.0.1 2020-02-27

- it's working
