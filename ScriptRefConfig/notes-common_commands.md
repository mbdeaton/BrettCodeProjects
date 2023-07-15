# A number of useful commands, specific to Windows Subsystem for Linux
Brett Deaton

Compilation begun 2010, continually updated

## NPM
npm start # run start script defined in package.json to spin up local server
lsof -i tcp:3000 # find the process id of the server listening to port 3000
kill -9 PID # kill the server running with process id PID
npm audit --production # ignore vulnerabilities from devDependencies (see
                         create-react-app issue #11174 for context)


## Python stuff
```
python -c "import os, sys; print(os.path.dirname(sys.executable))" # where is this executable from?
```
### Pip
```
pip install requests --index-url=<LOCAL PYPI MIRROR URI> # set local mirror of PyPI as source
### Virtual Environments
python3 -m venv example-venv # create a virtual environment in the current directory
example-venv/Scripts/activate # activate that virtual environment (precede call with `source` on *nix)
python # once virtual environment is activated, this will reference the local python interpreter
python -m piptools compile requirements.in # generate requirements.txt from core requirements
python -m pip install -r requirements.txt # install the specified libraries into the virtual environment
python -m pip install -e ./pkg # install local package pkg
deactivate # deactivate that virtual environment
```


## Docker
```
docker ps # list running containers, -a to include stopped containers
docker image ls # list images
docker run
docker container stop
docker rm <container id>
docker image rm <image id>
docker exec -it <container id> bash # open a bash shell inside a running container
docker attach # a similar command, recommended by michael sotolongo
```


## General Windows Powershell Commands
```
dir # same as bash ls\
cd # same as bash cd\
filename.md # open the file using its default interpreter also Invoke-Item filename.md\
Get-ChildItem | Where-Object Name -Like "*.ps1" # return only files with specific extension\
Get-ChildItem | Where-Object -filterscript {($_.Name -ge "IMG_3270") -and ($_.Name -le "IMG_3279)} # return only files within some name range\
Add-WindowsCapability -Online -Name Rsat.ActiveDirectory.DS-LDS.Tools~~~~0.0.1.0 # add module to query AD\
Get-ADPrincipalGroupMembership <username> -Server <server> | select name # query AD groups a user belongs to, default server is assumed here\
[Environment]::SetEnvironmentVariable("key", "value", [System.EnvironmentVariableTarget]::User) # modify user env variables permanently, use ::Machine to modify system vars
Add-LocalGroupMember -Group docker-users -Member "$env:UserName" # add a user to a local AD group
Get-ChildItem | Rename-Item -NewName { $_.Name -replace "Programming_", "" } # remove a string from a bunch of filenames\
Get-ChildItem -Exclude "Programming_*" | where-object -filterscript {$_.Name -ge "I"} | Rename-Item -NewName {"Programming_" + $_.Name} # append a prefix "Programming_" to all files with names alphabetically greater than or equal to "I"\
fc-list :outline -f "%{family}\n" | findstr /i "interstate" # find a font name (to use in pandoc)\
pandoc --pdf-engine=xelatex -f markdown -V colorlinks -V geometry:"top=2cm, bottom=1.5cm, left=2cm, right=2cm" -V mainfont="Calibri" --toc .\notes.md -o notes.pdf # pandoc convert markdown to pdf\
Get-History | Where-Object {$_.CommandLine -like "*keyword*"} # print commands used in this session including "keyword"\
```


## General CMD commands
```
net user /domain <username> :: find AD groups a user is part of, and much more
```


## Stream Editing
#### SED Glossary
```
#   -e	specifies the script you want to run on the first argument (the input file)
#		however without -e specified, the first argument after sed is assumed
#		to be the script.
#   -f	specifies the file you want to be interpreted as a sed script.
#   ''	these quotes are used instead of "" to escape shell expansion.
#   >	sed doesn't do anything to the input file, but pipes output to stdout. So to
#		use the commands below, you probably want to do "sed stuff > new.out"
#   -n	without this option, sed automatically prints the pattern space (the input
#		file less whatever the script has removed from it) to stdout. With
#		this option, sed only prints what the script says to print.
#   /	this is usually used as the separator for matching regexps. However whatever
#		character follows the s (replace) command is interpreted as the
#		separator and must be escaped in order to use it in its original
#		meaning. Thus "s:old:new:" is the same as "s/old/new/". And the former
#		is cleaner for pathnames e.g.: "s:/panfs/ds06/sxs/:/scratch/p/pfeiffer/:"
#		as opposed to "s/\/panfs\/ds06\/sxs\///scratch\/p\/pfeiffer\//"
#	careful about matching between characters on a line. sed automatically tries
#		to make the longest match possible. Thus "s/{.*}//g" transforms
#		"I{footnote} am{another footnote} I" -> "I I". A work-around for this
#		is to use the NOT operator for matches. Thus "s/{[^}]*}//g" transforms
#		"I{footnote} am{another footnote} I" -> "I am I"
#   ;	used to separate multiple commands in a single script. Each command is executed in
#		sequence on each line, then sed moves to the next line. Alternatively, you
#               can use multiple -e option flags.
#   \(	opening bracket used to delimit a logical region of a regexp, with \) as the closing
#		bracket. The logical regions can be referred to with \1, \2, and so on.
#		Thus "s/\(.*\) \(.*\) \(.*\)/\2 \1s/g" transforms
#		"Michael Brett Deaton" -> "Brett Michaels"
```

#### SED Commands
```
sed -e '1,10d' orig.out # delete lines 1-10 inclusive
sed -e '/pattern/,+7d' orig.out # delete from the line matching pattern to 7
#                                 lines later, inclusive. Add -i switch to do it
#                                 in place (changing orig.out).
sed -n -e '/Checkpoint/p' orig.out # print lines that have 'Checkpoint' in them
sed -n -e '/Checkpoint/!p' orig.out # print lines that don't have 'Checkpoint' in them
sed -n -e '/startpattern/,/endpattern/p' orig.out # print lines that are bracketed by startpattern and endpattern
sed -n '289185p' LS220_Tabulated-lowrho.dat # print the 289185th line
sed -n '/pattern/,$p' orig.out       # print everything from pattern to the end of the file
sed -n '/pattern/,/^\s*$/p' orig.out # print everything from pattern to the next blank line
sed -e 's/old/new/g' orig.out     # replace all instances of old with new
perl -p -i -e 's/old/new/g' orig.out # same but using perl; MAJOR DIFFERENCE: this overwrites orig.out with the output
sed -e 's/old/new/' orig.out      # same but only apply to first match to old on each line
sed -e '1,10s/old/new/g' orig.out # same but apply only to lines 1-10.
sed -e 's/.*/a line: &/' orig.out # append each line with "a line: ". The & refers to the matched regexp.
sed -e 's/$/\r/' myunix.txt # convert a unix style txt (with line feeds at ends of lines) to dos style
sed -e 's/.$//' mydos.txt   # convert a dos style txt (with line feeds and carriage returns) to unix style
# Other perl and perl-type search & replace examples
perl -lne 'print for /(\w*.png)/' magic_squares.html # find and print regex matches
sed -e 's/^t=\([^;]*\).*;SmoothLogMaximumDensity=\([^;]*\).*/\1  \2/' Hist-FuncSmoothLogMaximumDensity.txt > scratch.dat # transforms Hist-FuncSmoothLogMaximumDensity.txt to time & log density dat
grep -e '^#  And so' -e '^t=' SpEC.out | grep -A 1 '^#  And so' | sed -n -e 's/^t=\([^,]*\).*/\1/p' > evolution_pace.dat # or this crazy mess to get the start time at the beginning of each evolve segment
ls -1tr Lev1*/Run*/*Checkpoints/*/Source-Vars_Interval0.0.0.h5 | sed -e 's|^Lev1.*Checkpoints/\([^/]*\).*|\1|' # or this to get the timestep index in order for every checkpoint in Lev1
grep --exclude=DoMultipleRuns.input __ *input
grep -e "^t=" -e "^MPI" SpEC.out
grep -P Lev[0-9]- README # grep for any perl regexp Lev[0-9]- (e.g. this one excludes Lev[0-9]<space>)
grep "Output\s*=\s*MaximumDensity" *input # \s matches a whitespace char, \s* matches zero or more of them
grep -e '^#  And so' -e '^t=' SpEC.out | grep -A 1 '^#  And so' | sed -n -e 's/^t=\([^,]*\).*/\1/p' > merger_evolution_pace-Lev4f.dat # long pipe to get the evolution time at the beginning of each resubmission
egrep -v '^Error:' SpEC.out >> SpEC_temp.out # clean up a SpEC.out file with lots of 'Error:' statements
perl -p -i -e 's/Polytrope\/mns137/Polytrope\/Mergers\/mns137/g' DoMultipleRuns.input
perl -p -i -e 's/\s+/\n/g' inputfile.txt # convert a single row file to a single column file
perl -p -i -e 'print "# comment\n" if $. == 1' file.dat # insert comment at top of file (wrap in for; do; done to loop over multiple files)
perl -pi -e '$_ = "" if ($. == 1 );' -e 'print "#" if ($. == 1 )' Temp.yg # remove first line and comment out next line
# for getting input files from a different machine to work on a new machine, usually ~3 replaces are needed. Careful about order!
1) perl -p -i -e 's|=[^=]*Lev3Restart_a|=/panfs/ds06/sxs/mduez/LS/q4s.9/Lev3Restart_a|g' *input
2) perl -p -i -e 's|scratch/p/pfeiffer|panfs/ds06/sxs|g' *input
3) perl -p -i -e 's|home/p/pfeiffer|panfs/ds06/sxs|g' *input
# an awk parse of make output to get a list of the targets available:
make -qp | awk -F':' '/^[a-zA-Z0-9][^$#\/\t=]*:([^=]|$)/ {split($1,A,/ /);for(i in A)print A[i]}' | sort
PATH=a/b/c/d; UNWANTEDPREFIX=a/b; echo ${PATH#*$UNWANTEDPREFIX/} # the last command strips the unwanted prefix
tail -1 TStepperDiag.dat | awk '{ print $4 }' # this prints the last time step of a segment
awk '{ print $2,$5 }' out.dat > out-clean.dat # print only the 2nd and 5th columns of a file
awk '{ if( $1!="#" ){ print $0 } } # ignore comment lines (starting with #)
awk '$2=0.5' in.dat > out.dat # replace numbers in column 2 with 0.5
```


## Git things
```
git clone git@sxs-archive.tapir.caltech.edu:SpEC mySpECRepo # clone remote SpEC repo into local mySpECRepo
git init # start a new git repo in the current working dir (no files will be tracked until git add)
git tag -a <tagname> -m # create a tag object of a commit, refer to it later with <tagname>
git diff NeosPub:Hydro/HydroComputeItems/NeutrinoPhysics.cpp NeosPub2:Hydro/HydroComputeItems/NeutrinoPhysics.cpp # compare file in branch NeosPub to file in branch NeosPub2
git diff HEAD^ HEAD # compare most recent commit to previous
git diff HEAD^ -- DoMultipleRuns.input # compare specific file in most recent commit to previous
git diff --name-status 5d6c^..5d6c # compare (only file status) one commit, 5d6c, to immediate parent; this ordering of the commits gives the correct dependency flow: i.e. 'A' means the file was added by commit 5d6c
git switch feature; git rebase main # rebases current branch onto tip of main (identical to "git rebase main feature" from any branch)
git switch feature; git merge main # the preserving-history alternative to git rebase abvoe, creates a single new commit on feature that ties in the history of main at that point
git rebase -i HEAD^^ # perform an interactive rebase onto the 3rd most recent commit 
git log origin/master..HEAD # show what commits I'm about to push to remote master
git log --graph --oneline --branches --decorate # show a graphical representation of the local branches
git log --name-status # show the log and list modified files
git reflog # explicitly show all my actions, with related commit SHAs (useful for recovering from mistakes)
git reflog --date=iso # include commit dates
git ls-files # list all the files being tracked
git remote add foo-panfs shc.cacr.caltech.edu:/panfs/ds06/sxs/mbdeaton/foo # track remote repo and name it foo-panfs
git fetch foo-panfs # download entire repo history from remote foo-panfs
git checkout -b fie-branch foo-panfs/fie-branch # create new local branch tracking remote foo-panfs/fie-branch
git switch fie-branch # switch HEAD to new branch
git pull --rebase <remotename> <branchname> # pull from a different branch than the tracked branch
git pull --rebase -p # (or git fetch -p) also prune branches pointing to remotes that no longer exist
git push origin devel:user/deaton/AccretionLoss # push local branch devel to origin remote and name it AccretionLoss
git push origin +devel:user/deaton/AccretionLoss # push and allow non-fast-forward updates (this is necessary if I've rebased devel locally, maybe the same as push -f, but I don't think so)
git push -f origin NeutrinoAnnihilation:user/deaton/NeutrinoAnnihilation # force push (this is necessary if I've rebased NeutrinoAnnihilation locally, and want the remote to have that rebase (I'm deleting history))
git fetch; git reset --hard origin/user/deaton/NeutrinoAnnihilation # resets over local changes (which is what I want if I've done a git push -f and I want to retrieve the rewritten history from origin/user/deaton/NeutrinoAnnihilation)
git push origin --delete user/deaton/AccretionLoss # delete branch on origin remote
git push origin origin/usr/deaton/NeutrinoAnnihilation:refs/heads/user/deaton/NeutrinoAnnihilation # move remote branch usr/* to user/*
git branch -vv # look at local branches and see what remotes they're tracking
git branch -d branch_name    # delete local branch that has been merged
git branch -D branch_name    # delete local branch that hasn't been merged
git branch -r -d remote_name/branch_name # delete remote branch from local repo (This is also the way to make a local branch stop tracking remote_name/branch_name. If you want the remote branch back, just do a fetch remote_name.)
git branch --set-upstream NeutrinoAnnihilation origin/user/deaton/NeutrinoAnnihilation # reset local branch NeutrinoAnnihilation's remote tracking branch to the last arg (NOTE: the git syntax changes in 1.8)
git commit --amend # edit the commit message of your most recent commit
git rm # rm files from index and working tree (until re-added they are no longer going to be tracked)
git rm --cached # rm files from the index only (until re-added they are no longer going to be tracked)
git rebase HEAD^^; # set penultimate commit to 'edit'; git reset HEAD~; # chunk the files into separate commits; git rebase --continue # this is what you usually want to do to split the penultimate commit into separate commits
git reset <filename> # this is what you usually want to do rather than git rm, just un-adds the file
git reset --soft HEAD^  # undo last commit, but leave the working files as they were in the commit, and leave the index as it was just before the commit
git reset HEAD^         # undo last commit, but leave the working files as they were in the commit, and purge the index
git reset HEAD          # same as above, but just reset things to most recent commit
git reset --hard HEAD^  # undo last commit, and discard the changes to the working files
git revert <commit>    # create a new commit that reverses the changes from <commit>
git revert -n <commit> # prepare and add but don't commit a reversal of the changes from <commit>
git clean -f # clear out untracked files (if you also want to clear out ignored files (good mercy why!?) add -x
git config remote.fyodor-SpEC.uploadpack "/opt/local/bin/git-upload-pack" # this is required on any machine that wants to pull from my mac, called remote/fyodor-SpEC here, because my installation of git hides git-upload-pack from others?
git config --global alias.glog 'log --graph --oneline --branches --decorate' # add glog alias to ~/.gitconfig
# patches
git format-patch master --stdout > this_fix.patch # make a patch with a diff of this branch against master
git apply --stat this_fix.patch                   # examine (but don't apply) this patch's changes
git apply --check this_fix.patch                  # examine any errors that will come up by applying this patch
git apply this_fix.patch                          # actually apply the patch
# rebasing onto a branch who's history has changed (see spec-devel email 5.2.13)
git rebase --onto [newbase] [oldbase] # rebase --onto takes two args always; e.g. [newbase]=EvolveGeodesics, [oldbase]=EvolveGeodeics@{1} works if you're on NeutrinoAnnihilation and you've done a git fetch
git merge --ff-only feature # executed from master; this is the standard way to merge feature's commits to master before pushing
git stash list # show the stash stack
git stash drop stash@{1} # get rid of the penultimate stash
git switch develop; git checkout -b feature; git push --set-upstream-to origin feature; git commit -a -m 'Add feature'; git push; (create bitbucket PR; bitbucket merge PR) # gitflow at SEL
```


## General Bash commands
```
make 2>&1 | less -R # pipe make error output to less, output control colors in raw form (as colors not characters)
diff -qr thisdir thatdir # diffs all the way down to the bottom
diff -qw thisdir thatdir # -w ignores whitespace diffs
diff -w <(sed 's/;\s*)/)/g' HyDataBoxItems-a.input) <(sed 's/;\s*)/)/g' HyDataBoxItems-b.input) # ignore differences of semicola just before a closing paren
nohup make -s > make.out & # run a nohangup command, which continues even after logging out of a shell
ssh shcXXX killall -USR1 SpEC # send a clean kill command to all my SpEC processes on node shcXXX
for ((i=0; i<10; ++i)); do tail -1 Errors${i}.txt; done # this does what I want 'tail -1 Errors?.txt' to do.
for dir in Lev1_A?; do ls -d $dir; done # a similar thing but looping over directories
for file in endpoints-nue-all-0??.dat; do tail -n +10 $file >> endpoints-nue-all.dat; done # concatenate all but the top 9 lines of the files
ls -I "*h5" -I "Cp-*" -I "Link*" | xargs rm # use xargs to pass a list of files to delete to rm. Doesn't work with rm -i
sed -e 's|^|../../|g;s|/$|/Evolution.input|g' RunDirs.input | xargs -d '\n' ls -d # use xargs to pass a list of run directories to some other argument (ls in this case)
ls -R | grep ":$" | sed -e 's/:$//' -e 's/[^-][^\/]*\//--/g' -e 's/^/   /' -e 's/-/|/' # make an ascii visualization of the directory structure
ps2pdf -dEPSCrop unbound_velocity_histogram.eps # converts a ps or eps to pdf (to use in pdflatex) great compression! The dEPSCrop option maintains the bounding box frame size of the postscript
ps2eps -B 3D-Q0phys_meridional.ps # converts to eps (e.g. for apj submission). The -B option maintains the bounding box frame size of the postscript
convert -loop 0 Images/rho0phys-disk-Lev4l0*.png test.gif # convert a bunch of pngs to an animated gif
# Rename Utility. There are different rename utilities on different machines
rename old new target                 # on zwicky/shc/etc: e.g. rename Lev1- Lev1-35- Lev?-0
rename -n 's/Lev1-/Lev1-35-/g' Lev?-0 # on hplus/fyodor: -n prevents any changes from being made, just lists what would have been changed
find . -name '*.dSYM' -exec rm -rf "{}" \; # find and delete a bunch of dSYM debugging symbol dirs
# remote file mount
1) $ mkdir <somename>
2) $ sshfs -o follow_symlinks -o transform_symlinks zwicky.cacr.caltech.edu:/panfs/ds06 <somename>
3) fusermount -u <somename> # unmount, on ubuntu
3) umount <somename>        # unmount, on mac os, actually, this doesn't seem to work either
# tar
tar -cvf pviewf.tar pviewfiles* # Be careful with tarring. If you just give one argument (and -c) you can overwrite the existing files
tar -cvf /home/mbdeaton/scratch/MattEOSdriver.tar --exclude=*dat --exclude=*h5 * # exclude some files from the tarball
tar -xvf pviewf.tar # extract that shit!
# Make movies from a series of stills. In this example, stills are named L_nu.????.jpg.
#   Currently, this only works on hplus. I can't find all the needed modules on fyodor.
1) ffmpeg -i 'L_nu.%04d.jpg' -pix_fmt yuv420p -f yuv4mpegpipe L_nu-Movie.yuv
2) x264 --crf 20 --preset veryslow --tune animation --keyint 6 --fps 6 --profile main --level 3.1 --vbv-maxrate 14000 --vbv-bufsize 14000 --non-deterministic --output L_nu-Movie.mp4 --demuxer y4m L_nu-Movie.yuv # --fps argument is frames per second
3) MP4Box -inter 500 L_nu-Movie.mp4
# Mounting drives and network locations on WSL
mkdir /mnt/z # first make the mount point
sudo mount -t drvfs Z: /mnt/z # mount external drive or mapped network drive Z to standard location
sudo umount /mnt/z # safely unmount the z drive; note this appears to be done automatically when I close my WSL ubuntu session
host <url> # perform a DNS lookup, returning the IP address
```


## gnuplot
```
# to plot columns from different files use < join. It pastes the second
#   file's columns to the right of the first file's
p '<join ControlExpansion.dat ControlPivotExpansion.dat' u 1:($2*$10) w l
# for instance, to plot the inspiral in polar coords
plot '< join ControlRotation.dat ControlExpansion.dat' u ($10*cos($2)):($10*sin($2)) w l
# or plot the relative error in density
p '< join RhoErr_x.dat RhoEx.dat' u 1:($2/$3)
# to plot some column ordinally (for instance if there is no ordinal value in the file),
#   and furthermore, to plot only every other line starting at the zeroeth
p 'snes.dat' every 2::0 u 2
# to do the same but starting at the next line
p 'snes.dat' every 2::1 u 2
# to plot the time evolution of a profiler field (2) on the 1st (0th) of 72 nodes
p 'Profiler.dat' u 0:2 ev 72::0
# In general the register range specification goes like ev :::::, where we have
#   point_incr : block_incr : point_start : block_start : point_end : block_end
# to draw a vertical dashed line at x=4 tagged 1 (for later deletion or reference)
set arrow 1 from 4,graph 0 to 4,graph 1 nohead ls 0
# to force a certain linestyle instead of ls use lt (linetype)
p 'Lev2/Run/ControlExpansion.dat' w l lt 1, 'Lev2_a/Run/ControlExpansion.dat' w l lt 1
# to plot using two y axes (you can also plot with the 2nd x axis, x2)
p 'Rho.dat' w l axes x1y1, 'Lnu.dat' w l axes x1y2
set yrange [0:0.008]
set ytics nomirror
set y2range [1e45:1e55]
set y2tics nomirror
set logscale y2
# plot a histogram from ygraph type output: [1] bound on g, [2] sum f
# 3 styles depend on whether [1] represents lower, upper, or avg of the bin boundaries
p 'NewtFallBackTimeHistogram.dat' u 1:2 w fsteps  # [1] is upper bound
p 'NewtFallBackTimeHistogram.dat' u 1:2 w steps   # [1] is lower bound
p 'NewtFallBackTimeHistogram.dat' u 1:2 w histeps # [1] is (upper-lower)/2
set key top left # put the legend on the other side
set term pngcairo size 640,480 enhanced dashed font "Arial,12" # nice bitmap format for the sxs wiki
# Or even better than bitmap, svg: but there are some tricks
set term svg size 640,480 enhanced dashed font "Arial,12" # nice format for the sxs wiki
# sometimes when the svg is loaded by a webbrowser you get an error of the form,
# "Extra content at the end of the document". I've found I can get rid of this error by deleting
# a newline at the end of the svg file. Also, this error doesn't seem to present itself when
# I've already done one "set output" and am making a second or third svg of a different filename.
set palette rgbformulae r,g,b # The three numbers refer to functions assigned to r,g,b respectively.
#                               The functions are hardcoded; list them with show palette rgbformulae.
#                               The input x ranges from 0-1 (this is mapped to the range given by
#                               set cbrange). The output is on a range of 0-1, 0 being black, 1 being
#                               saturated r, g, or b. A minus sign in front of a function number means
#                               the input reverses range, from 1-0. Negative output is treated as zero.
set palette rgbformulae 8,-36,-5 # thermometer colors emphasizing the cold end
set palette rgbformulae 8,16,-5 # thermometer colors emphasizing both ends
set datafile commentschars '"' # set comment character to " (as in *yg files)
p 'data.dat' u 1:2:3 w lp palette # plot the color of the line according to column 3
```
