#!/bin/sh
set -x 
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/project
/project/svdemo --config-file=/project/caf-application.conf
