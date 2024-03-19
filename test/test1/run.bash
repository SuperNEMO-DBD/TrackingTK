#!/usr/bin/env bash
work_dir=""
cfg_dir=""
with_visu=0

function parse_cl_opts()
{
    while [ -n "$1" ]; do
	local opt="$1"
	if [ "${opt}" == "--work-dir" ]; then
	    shift 1
	    work_dir="$1"
	elif [ "${opt}" == "--cfg-dir" ]; then
	    shift 1
	    cfg_dir="$1"
	elif [ "${opt}" == "--visu" ]; then
	    with_visu=1
	else
	    echo >&2 "[error] Invalid command line option '${opt}'! Abort!"
	    return 1
	fi
	shift 1
    done
    return 0
}

parse_cl_opts $@
if [ $? -ne 0 ]; then
    exit 1
fi

if [ -z "${work_dir}" ]; then
    work_dir="_tmp_work.d"
fi
if [ -z "${cfg_dir}" ]; then
    cfg_dir="."
fi
export FLWORKDIR="${work_dir}"

echo >&2 "[info] cfg_dir   = '${cfg_dir}'"
echo >&2 "[info] work_dir  = '${work_dir}'"
echo >&2 "[info] FLWORKDIR = '${FLWORKDIR}'"

function my_exit()
{
    local error_code="$1"
    shift 1
    local error_msg="$@"
    if [ -n "${error_msg}" ]; then
	echo >&2 "[error] $@"
    fi
    # if [ -d ${FLWORKDIR} ]; then
    # 	tree ${FLWORKDIR}
    # 	echo >&2 "[info] removing work dir '${FLWORKDIR}'..."
    # 	rm -fr ${FLWORKDIR}
    # fi
    exit ${error_code}
}

which flsimulate > /dev/null 2>&1
if [ $? -ne 0 ]; then
    my_exit 1 "flsimulate is not available! Abort!"
fi

which flreconstruct > /dev/null 2>&1
if [ $? -ne 0 ]; then
    my_exit 1 "flreconstruct is not available! Abort!"
fi

if [ ! -d ${FLWORKDIR} ]; then
    mkdir -p ${FLWORKDIR}
fi

cp ${cfg_dir}/variant.profile ${FLWORKDIR}/
cp ${cfg_dir}/simu.conf ${FLWORKDIR}/
cp ${cfg_dir}/reco.conf ${FLWORKDIR}/
echo >&2 "[info] FLWORKDIR directory:"
tree ${FLWORKDIR}/

# exit 0

flsimulate --verbosity debug \
	   --config "${FLWORKDIR}/simu.conf" \
	   --run-number 12 \
	   --first-event-number 0 \
	   --output-file sd.brio \
	   > ${FLWORKDIR}/simu.log 2>&1
if [ $? -ne 0 ]; then
    cat ${FLWORKDIR}/simu.log
    my_exit 1 "flsimulate failed"
fi

flreconstruct --verbosity debug \
	   --config "${FLWORKDIR}/reco.conf" \
	   --input-file sd.brio \
	   --output-file rec.brio \
	   > ${FLWORKDIR}/reco.log 2>&1 
if [ $? -ne 0 ]; then
   cat ${FLWORKDIR}/reco.log
   my_exit 1 "flreconstruct failed"
fi

tree ${FLWORKDIR}/

if [ -f rec.brio ]; then
    echo >&2 "[info] Dump REC output file:"
    flreconstruct --verbosity debug --input-file rec.brio
fi

my_exit 0

exit 0
