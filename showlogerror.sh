# by yuwei5 2013-11-4

cd /usr/local/httpd/logs

echo "Segmentation fault"
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn "Segmentation fault" error_log*| grep "`date '+%b %d' --date='0 day'`"|wc -l

echo "http 500"
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn " 500" error_log*| grep "`date '+%b %d' --date='0 day'`"|grep "http"|wc -l
echo

