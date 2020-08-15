define timen
shell echo set \$starttime=$(perl -MTime::HiRes=time -e 'printf "%.6f\n", time') > /tmp/gdbtmp
source /tmp/gdbtmp
n
shell echo print $(perl -MTime::HiRes=time -e 'printf "%.6f\n", time')-\$starttime-0.02 > /tmp/gdbtmp
source /tmp/gdbtmp
end
define timec
shell echo set \$starttime=$(perl -MTime::HiRes=time -e 'printf "%.6f\n", time') > /tmp/gdbtmp
source /tmp/gdbtmp
c
shell echo print $(perl -MTime::HiRes=time -e 'printf "%.6f\n", time')-\$starttime-0.02 > /tmp/gdbtmp
source /tmp/gdbtmp
end
