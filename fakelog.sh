while [ 1 ]; do  
    echo "[T] Jan 06 18:27:3262014] [werror] child process 31285 still did not exit, sending a SIGKILL" >> /usr/local/httpd/logs/error_log
    echo "[T] Jan 06 18:27:3262014] [werror] child process 31285 still did not exit, sending a SIGKILL" >> /usr/local/httpd/logs/error_log
    echo "[T] Jan 06 18:27:3262014] [warning] child process 31285 still did not exit, sending a SIGKILL" >> /usr/local/httpd/logs/error_log
    echo "Mon Jan 06 18:27:37 2014] [notice] caught SIGTERM, shutting down" >> /usr/local/httpd/logs/error_log
    echo "[Fri Dec 27 15:30:08 2013] [notice] seg fault or similar nasty error detected in the parent process" >> /usr/local/httpd/logs/error_log
    echo "[Fri Dec 27 15:30:10 2013] [notice] Apache/2.2.25 (Unix) PHP/5.3.27 configured -- resuming normal operations" >> /usr/local/httpd/logs/error_log
    echo "[Fri Dec 27 15:40:09 2013] [error] [client 127.0.0.1] client denied by server configuration: /usr/local/httpd/htdocs/sd/conf/" >> /usr/local/httpd/logs/error_log
    curl http://127.0.0.1 > /dev/null
    curl http://127.0.0.1/h.php > /dev/null
    curl http://127.0.0.1/ha.php > /dev/null
    curl http://127.0.0.1/hac.php > /dev/null
    curl http://127.0.0.1/hack.php > /dev/null
    curl http://127.0.0.1/hacka.php > /dev/null
    curl http://127.0.0.1/hackat.php > /dev/null
    curl http://127.0.0.1/hackath.php > /dev/null
    curl http://127.0.0.1/hackatho.php > /dev/null
    curl http://127.0.0.1/hackathon.php > /dev/null
    curl http://127.0.0.1/hackatho.php > /dev/null
    curl http://127.0.0.1/hackath.php > /dev/null
    curl http://127.0.0.1/hackat.php > /dev/null
    curl http://127.0.0.1/hack.php > /dev/null
    curl http://127.0.0.1/hac.php > /dev/null
    curl http://127.0.0.1/ha.php > /dev/null
    curl http://127.0.0.1/h.php > /dev/null
    sleep 1
done  
