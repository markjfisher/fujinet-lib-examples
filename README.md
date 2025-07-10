# fujinet-lib-examples

The way to test, exercise and explore the use of FujiNet via the fn-lib libraries is here in this repo.

## dir structure

This repo requires that projects are in the 2nd directory deep from the root folder, and that directory is the project's root.
This is to satisfy the build paths for finding the `makefiles` directories correctly.

Each root folder is split into the major fujinet devices; `clock`, `fuji`, `network`.

Sub-folders under these are the application root folders (see below), e.g. `fuji/base64`.
These folders contain the core Makefile for that application. You can copy one from an existing folder to get going and simply
change the TARGETS list.

```none
.
├── clock
│   ├── get_time
│   └── time_loop
├── fuji
│   ├── appkey
│   ├── apple-sp-info
│   ├── base64
│   └── hashing
└── network
    ├── echo-test
    ├── httpbin
    ├── http-leak-analysis
    ├── json-test
    ├── mastodon
    ├── multi-net
    ├── net-read
    └── non-blocking-demo
```

## duck - webdav copy

The following allows you to copy files from your file system to the SD card of your fujinet for easy testing on real devices.

This allows you to compile locally, then copy the app file to the SD, boot the fujinet and run the new application. Alternatively see the `testing` section below for running under emulation.

```shell
duck --upload dav://anonymous@fujinet.home/dav/target-file source-file -existing overwrite

# copying from SD to local
duck -d dav://anonymous@fujinet.home/dav/target-file /path/to/local/file
```

## building applications

All apps have their own Makefile, to start a build for that app for supported platforms, run the following from the app's root folder:

```shell
make clean && make release disk
```

## supported platforms

Each apps Makefile defines the TARGETS value for supported platforms to compile.

## testing

You need to setup your preferred emulator yourself, e.g. Altirra for Atari, vice for c64.

For atari, ensure you have `ALTIRRA_HOME` configured.

The configuration requirements can be found in each `custom-<platform>.mk` file in the makefiles dir.

If your platform has an emulator that it can use, simply running `make test` should start it.
Setting up fujinet-pc is beyond the scope of this readme.

## copying files to webdav

### rclone

On linux, install `rclone` and configure a webdav config with:

```shell
$ rclone config create fujinet webdav url http://fujinet.home/dav vendor other
```

substitute your fujinet address in place of fujinet.home.

Now you can copy files on the command line with something like:

```shell
$ rclone copyto dist/netread.atari.com fujinet:atari/netread.xex
```

This will copy the dist/netread.atari.com file and place it in the fujinet's "atari" dir, renamed to "netread.xex"

Full compilation and copy for convenience:

```shell
make clean && \
  make TARGETS=atari release && \
  rclone deletefile fujinet:atari/netread.xex && \
  rclone copyto dist/netread.atari.com fujinet:atari/netread.xex
```
