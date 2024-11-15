#! /bin/bash

set -u

DIR="$(dirname "$(readlink -f "$0")")"
BASE_PATH="$DIR/../.."
cd "$BASE_PATH" || exit

GREEN='\033[0;32m'
RED='\033[0;31m'
OFF='\033[0m'
STATUS=0

echo "Compiler should accept..."
echo ""

for test in $(find src/test/c/accept -maxdepth 2 -type f | sort); do
	build/Compiler >/dev/null 2>&1 <"$test"
	RESULT="$?"
	if [ "$RESULT" == "0" ]; then
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it rejects${OFF} (status $RESULT)"
	fi
done
echo ""

echo "Compiler should reject..."
echo ""

for test in $(find src/test/c/reject -maxdepth 2 -type f | sort); do
	build/Compiler >/dev/null 2>&1 <"$test"
	RESULT="$?"
	if [ "$RESULT" != "0" ]; then
		echo -e "    $test, ${GREEN}and it does${OFF} (status $RESULT)"
	else
		STATUS=1
		echo -e "    $test, ${RED}but it accepts${OFF} (status $RESULT)"
	fi
done
echo ""

echo "All done."
exit $STATUS
