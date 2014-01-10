# by yuwei5 2013-11-4

cd /usr/local/httpd/logs

date +%Y-%m-%d
echo -------------------------------------------------
echo "not found"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-3 day'`"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-2 day'`"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-1 day'`"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d'`"|wc -l

echo -------------------------------------------------
echo "class not found"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-3 day'`"|grep "lass"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-2 day'`"|grep "lass"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d' --date='-1 day'`"|grep "lass"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn "not found" error_log.*| grep "`date '+%b %d'`"|grep "lass"|wc -l


echo -------------------------------------------------
echo "Segmentation fault"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn "Segmentation fault" error_log*| grep "`date '+%b %d' --date='-3 day'`"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn "Segmentation fault" error_log*| grep "`date '+%b %d' --date='-2 day'`"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn "Segmentation fault" error_log*| grep "`date '+%b %d' --date='-1 day'`"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn "Segmentation fault" error_log*| grep "`date '+%b %d' --date='0 day'`"|wc -l

echo -------------------------------------------------
echo "http 500"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn " 500" error_log*| grep "`date '+%b %d' --date='-3 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn " 500" error_log*| grep "`date '+%b %d' --date='-2 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn " 500" error_log*| grep "`date '+%b %d' --date='-1 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn " 500" error_log*| grep "`date '+%b %d' --date='0 day'`"|grep "http"|wc -l
echo
echo -------------------------------------------------
echo "http 502"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn " 502" error_log*| grep "`date '+%b %d' --date='-3 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn " 502" error_log*| grep "`date '+%b %d' --date='-2 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn " 502" error_log*| grep "`date '+%b %d' --date='-1 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn " 502" error_log*| grep "`date '+%b %d' --date='0 day'`"|grep "http"|wc -l
echo
echo -------------------------------------------------
echo "http 504"
echo -e "`date +%m-%d --date='-3 day'`: \c"
grep -rn " 504" error_log*| grep "`date '+%b %d' --date='-3 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-2 day'`: \c"
grep -rn " 504" error_log*| grep "`date '+%b %d' --date='-2 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-1 day'`: \c"
grep -rn " 504" error_log*| grep "`date '+%b %d' --date='-1 day'`"|grep "http"|wc -l
echo -e "`date +%m-%d --date='-0 day'`: \c"
grep -rn " 504" error_log*| grep "`date '+%b %d' --date='0 day'`"|grep "http"|wc -l
echo

