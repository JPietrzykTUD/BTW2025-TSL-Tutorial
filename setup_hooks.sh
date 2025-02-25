#!/bin/bash
cp .githooks/* .git/hooks/
chmod +x .git/hooks/*


#git config core.hooksPath .githooks