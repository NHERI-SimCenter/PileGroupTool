#!/bin/bash

#
# set system specific configuration
#

APP=PileGroupTool
QWT_FRAMEWORK=qwt.framework
QWT_FRAMEWORK_PATH=/usr/local/qwt-6.1.4-svn/lib/

QT_DEPLOY=/Users/pmackenz/Qt/5.11.2/clang_64/bin/macdeployqt
#
# ---- do not change anything below this line -----------------------------------
#

#
# identify release build directory
#

LONG=`grep BuildConfiguration $APP.pro.user | grep Release`
START=${LONG#*">"}
build_folder=${START%"<"*}

if [ -d $build_folder ]
then
    (
	cd $build_folder
	echo "switching to $PWD"

	if [ -d $APP.app ]
	then

	    QWT_LIB=`otool -L $APP.app/Contents/MacOS/$APP | grep qwt | cut -d\  -f1 `
	    QWT_LIB=${QWT_LIB//[[:space:]]/}
	    
	    echo $QWT_LIB

	    if [ ${QWT_LIB%%/*} == "@executable_path" ]
	    then
                echo
		echo "*** looks like this script already ran on this package   ***"
		echo "*** do a clean project -> qmake -> build project to redo ***"
		echo
		exit 9
	    fi

	#
	# check if qwt.framework is installed
	#

	    if [ -d $QWT_FRAMEWORK_PATH/$QWT_FRAMEWORK ]
	    then
		echo "verifying that $QWT_FRAMEWORK exists: OK"
	    else
		echo "verifying that $QWT_FRAMEWORK exists: FAILED"
		exit 1
	    fi

	#
	# copy QWT framework to $APP.app
	#

	    if [ ! -d $APP.app/Contents/Frameworks ]
	    then
		mkdir $APP.app/Contents/Frameworks
	    fi
	    
	    if [ -d $APP.app/Contents/Frameworks/$QWT_FRAMEWORK ]
	    then
		echo "replacing ${QWT_FRAMEWORK} in $APP.app folder"
		rm -r $APP.app/Contents/Frameworks/$QWT_FRAMEWORK
	    else
		echo "installing ${QWT_FRAMEWORK} in $APP.app folder"
	    fi
	    cp -R $QWT_FRAMEWORK_PATH/$QWT_FRAMEWORK $APP.app/Contents/Frameworks/.

	    install_name_tool -id @executable_path/../Frameworks/$QWT_LIB $APP.app/Contents/Frameworks/$QWT_LIB
	    install_name_tool -change $QWT_LIB  @executable_path/../Frameworks/$QWT_LIB  $APP.app/Contents/MacOS/$APP 

	    if [ -f $APP.dmg ]
	    then
		mv $APP.dmg $APP.dmg~
	    fi

	#
	# creating the dmg package
	#

	    if [ ! -x $QT_DEPLOY ]
	    then
		echo "no executable macdeploy tool found"
		exit 1
	    fi

	    $QT_DEPLOY $APP.app -fs=HFS+ -dmg

	    echo done.

	else

	    echo
	    echo "*** Build Release version of PileGroupTool before running this script. ***"
	    echo
	    exit 1

	fi
    )

else
    echo
    echo "*** cannot find $build_folder ***"
    echo
    exit 2
fi

