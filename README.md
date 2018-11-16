# ocaml-ssh

### Installation

Not yet in OPAM repository, installation from git required.

```bash
apt install libssh-dev
opam pin ssh https://github.com/matt2134711/ocaml-ssh.git#master
```

### Usage Examples

See [example](example/) directory in source tree.

### Tips

* Re-use sessions and channels where possible
* Reduce the default SSH timeout
* ...

### License

`ocaml-ssh` released under the terms of the ISC license. See [LICENSE](LICENSE)
in project root for more information.

Links to `libssh` which is released under the terms of the LGPL 2.1 license. See
`COPYING` file in libssh source tree for more information.
