#!/bin/bash
echo "Content-type: text/html"
echo ""
echo "<html><body>"
echo "<h1>Hello from Bash CGI!</h1>"
echo "<p>Current date and time: $(date)</p>"
echo "</body></html>"