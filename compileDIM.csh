#!/usr/bin/tcsh
#Source: https://dim.web.cern.ch/dim/dim_unix.html
#Dim needs libmotif-dev installed: 
#Fedora: dnf install motif-devel
#Ubuntu: sudo apt-get install libmotif-dev tcsh libxt-dev

setenv OS Linux
setenv CC g++
#building path is created as DIMDIR/ODIR
setenv BASENAME dim_v20r15
setenv ODIR obj
setenv DIMDIR .

echo ' => Installing DIM in ' ${BASENAME} '...'
curl https://dim.web.cern.ch/dim/${BASENAME}.zip -o ${BASENAME}.zip && \
  unzip ${BASENAME}.zip && \
  chmod 755 dim_v20r15 -R
if ( $status > 0 ) then
  echo " => Error unable to retrive remote dim files.."
  exit 1
endif

cd ${BASENAME}/ && \
  mkdir ${ODIR} &&\
  make
if ( $status > 0 ) then
  echo " => Error cannot compile.."
  exit 1
endif

#Custom make install 
echo ' => Moving ' ${ODIR} ' to /usr/local/include/'
echo ' => Moving ' ${ODIR} '/libdim.e and libdim.so  to /usr/local/lib64/'
cp -R dim/ /usr/local/include/ && \
  cp ${ODIR}/libdim.a ${ODIR}/libdim.so /usr/local/lib/

if ( $status > 0 ) then
  echo " => Error Moving libraries you don't have the permissions.."
  exit 1
endif

echo ' => Dim has been installed!'
exit 0
