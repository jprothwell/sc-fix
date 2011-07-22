#!/bin/sh
# must be sourced with bash

# the SOFT_WORKDIR_CLEAN_ENV variable is set to a script responsible to cleanup some stuff before setting a new env (mainly the path)
if [[ -n "$SOFT_WORKDIR_CLEAN_ENV" ]]; then
    test -f $SOFT_WORKDIR_CLEAN_ENV.sh && source $SOFT_WORKDIR_CLEAN_ENV.sh
fi

## Set perl environment variables
export PERL5LIB=${SOFT_WORKDIR}/env/utils/lib/
export PERL_RL="Perl"
export PERL_READLINE_NOWARN=1

if [[ $(uname | grep -c CYGWIN) > 0 ]]; then  
    export PATH="${SOFT_WORKDIR}/env/win32:${PATH}"
fi

export PATH="${SOFT_WORKDIR}/env:$PATH"
export PATH="${SOFT_WORKDIR}/env/utils:$PATH"
export PATH="${SOFT_WORKDIR}/env/compilation:$PATH"
export PATH="${SOFT_WORKDIR}/env/deliv:$PATH"
export SOFT_WORKDIR_CLEAN_ENV="${SOFT_WORKDIR}/env/clean_env"

# use the OBJECT_DIR stuff on helios or vulcain
# also create the directory if needed
if [ "$HOSTNAME" == "helios" ] || [ "$HOSTNAME" == "vulcain" ] || [ "$HOSTNAME" == "petrus" ] || [ "$HOSTNAME" == "jupiter" ] || [ "$HOSTNAME" == "pluton" ] || [ "$HOSTNAME" == "sancerre" ] || [ "$HOSTNAME" == "medoc" ] || [ "$HOSTNAME" == "bergerac" ] || [ "$HOSTNAME" == "cadillac" ]; then
    export OBJECT_DIR=/tmp/${USER}_obj/${PROJ}/`echo ${SOFT_WORKDIR/$PROJ_WORKDIR\//} | tr "/" "_"`
    if [ ! -d $OBJECT_DIR ]; then
        mkdir -p $OBJECT_DIR
    fi
fi

alias cds='cd ${SOFT_WORKDIR}'
alias cde='cd ${SOFT_WORKDIR}/env'
alias cdenv='cd ${SOFT_WORKDIR}/env'
alias cdpal='cd ${SOFT_WORKDIR}/platform/chip/pal'
alias cdp='cd ${SOFT_WORKDIR}/platform/chip/pal'
alias cdphy='cd ${SOFT_WORKDIR}/platform/phy'
alias cdhal='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdh='cd ${SOFT_WORKDIR}/platform/chip/hal'
alias cdedrv='cd ${SOFT_WORKDIR}/platform/edrv'
alias cdboot='cd ${SOFT_WORKDIR}/platform/chip/boot'
alias cdrom='cd ${SOFT_WORKDIR}/platform_test/hal/int_rom_programmer'
alias cdspp='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spp'
alias cdspc='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spc'
alias cdspal='cd ${SOFT_WORKDIR}/platform/chip/bcpu/spal'
alias cdrfd='cd ${SOFT_WORKDIR}/platform/edrv/rfd'
alias cdsx='cd ${SOFT_WORKDIR}/platform/base/sx'
alias cdstack='cd ${SOFT_WORKDIR}/platform/stack'
alias cdtest='cd ${SOFT_WORKDIR}/platform_test'
alias cdtst='cd ${SOFT_WORKDIR}/platform_test'
alias cdt='cd ${SOFT_WORKDIR}/platform_test'
alias cdproto='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdpro='cd ${SOFT_WORKDIR}/platform_test/stack/proto'
alias cdcal='cd ${SOFT_WORKDIR}/platform_test/calib/calib_stub'
alias cdhex='cd ${SOFT_WORKDIR}/hex'
alias cdchip='cd ${SOFT_WORKDIR}/platform/chip'
alias cdbcpu='cd ${SOFT_WORKDIR}/platform/chip/bcpu'

# cdregs
function cdregsFunc {
    asic_name=`make -C ${SOFT_WORKDIR}/platform get_var_CT_ASIC | grep -v "make:"`
    if [ ! -d "${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include" ]; then
        cd ${SOFT_WORKDIR}/platform/chip/regs;
    else
        cd ${SOFT_WORKDIR}/platform/chip/regs/${asic_name}/include;
    fi
}

alias cdregs='cdregsFunc'

alias cddefs='cd ${SOFT_WORKDIR}/platform/chip/defs'
alias cdrom='cd ${SOFT_WORKDIR}/platform/chip/rom'
alias cdfmd='cd ${SOFT_WORKDIR}/platform/edrv/fmd'
alias cdmcd='cd ${SOFT_WORKDIR}/platform/edrv/mcd'
alias cdumss='cd ${SOFT_WORKDIR}/platform/svc/umss'
alias cdsvc='cd ${SOFT_WORKDIR}/platform/svc'
alias cductls='cd ${SOFT_WORKDIR}/platform/svc/uctls'
alias cdmps='cd ${SOFT_WORKDIR}/platform/svc/mps'
alias cdmrs='cd ${SOFT_WORKDIR}/platform/svc/mrs'
alias cdimgs='cd ${SOFT_WORKDIR}/platform/svc/imgs'
alias cdcams='cd ${SOFT_WORKDIR}/platform/svc/cams'
alias cdavps='cd ${SOFT_WORKDIR}/platform/svc/avps'
alias cdavrs='cd ${SOFT_WORKDIR}/platform/svc/avrs'
alias cdcamd='cd ${SOFT_WORKDIR}/platform/edrv/camd'
alias cdtarget='cd ${SOFT_WORKDIR}/target'

alias cdmmi='cd ${SOFT_WORKDIR}/application/coolmmi/main'
alias cdm='cd ${SOFT_WORKDIR}/application/coolmmi/main'

echo "Soft base SOFT_WORKDIR=$SOFT_WORKDIR"

# usefull git function

alias gitk='gitk --all'

