#!/bin/bash

HOME=$(git rev-parse --show-toplevel)

echo -n "Formating the code with Artistic Style ... "
astyle --style=google --indent=spaces=4 --indent-modifiers --indent-switches --indent-preproc-block --indent-preproc-define --indent-namespaces --indent-col1-comments --pad-oper --pad-comma --pad-header --align-pointer=type --align-reference=name --add-brackets --convert-tabs --close-templates --lineend=linux --preserve-date --suffix=none --formatted ${HOME}/src/* > /tmp/out.log 2>&1 || exit 1
echo "Done."

[ -s /tmp/out.log ]
if [ $? -eq 0 ]; then
	echo -e "\033[0;34m"
	cat /tmp/out.log
	echo ""
	echo -e "\033[0;41mPlease commit (or not) these changes and run the script again before to commit.\033[0;0m"
	echo ""
	exit 1
fi
