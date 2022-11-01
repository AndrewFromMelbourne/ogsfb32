#!/usr/bin/env bash

source /etc/os-release

RELEASE="UNKNOWN"

case $ID in

  debian)
    RELEASE="DEBIAN"
    ;;

  ubuntu)
    RELEASE="UBUNTU"
    ;;

  *)
    RELEASE="UNKNOWN"
    ;;
esac

cat << EOF > linuxRelease.h
#pragma once
#define $RELEASE
EOF
