#!/bin/sh

export PYTHONPATH=../python/build/lib.linux-x86_64-3.6:$PYTHONPATH
exec "$@"