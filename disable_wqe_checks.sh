#!/bin/bash

dev=05:00.1

mcra $dev 0x24908 0
mcra $dev 0x2490c 0
mcra $dev 0x24910 0
mcra $dev 0x24914 0
mcra $dev 0x24918 0
mcra $dev 0x2491c 0

mcra $dev 0x24908,24

mcra $dev 0x24928 0
mcra $dev 0x2492c 0
mcra $dev 0x24930 0
mcra $dev 0x24934 0
mcra $dev 0x24938 0
mcra $dev 0x2493c 0

mcra $dev 0x24928,24
