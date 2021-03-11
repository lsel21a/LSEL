for dir in ./*/
do
  dir=${dir%*/}

  if test -f "${dir##*/}/project.yml";
  then
    echo "Ceedling project found @ ${dir%*/}. Generating coverage... "
    cd "${dir##*/}/"

    if ceedling gcov:all utils:gcov;
    then
      echo "Coverage @ ${dir%*/} Ok!"
    else
      echo "Coverage @ ${dir%*/} failed!"
    fi

    cd ..
  fi
done