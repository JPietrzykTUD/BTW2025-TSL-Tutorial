#!/bin/bash
sudo mkdir -p /usr/include/tsl
sudo curl -L -s "https://github.com/db-tu-dresden/TSL/releases/latest/download/setup_tsl.sh" | /bin/bash -s -- --system