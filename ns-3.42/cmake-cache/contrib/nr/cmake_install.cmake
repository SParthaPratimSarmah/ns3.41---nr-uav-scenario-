# Install script for directory: /home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "default")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so"
         RPATH "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE SHARED_LIBRARY FILES "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/build/lib/libns3.42-nr-default.so")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so"
         OLD_RPATH "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/build/lib::::::::::::::::::::::::"
         NEW_RPATH "/usr/local/lib:$ORIGIN/:$ORIGIN/../lib:/usr/local/lib64:$ORIGIN/:$ORIGIN/../lib64")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libns3.42-nr-default.so")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/ns3" TYPE FILE FILES
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-phy-rx-trace.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-mac-rx-trace.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-point-to-point-epc-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-bearer-stats-calculator.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-bearer-stats-connector.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-bearer-stats-simple.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/beamforming-helper-base.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/ideal-beamforming-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/realistic-beamforming-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/node-distribution-scenario-interface.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/grid-scenario-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/hexagonal-grid-scenario-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/file-scenario-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/cc-bwp-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-radio-environment-map-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-spectrum-value-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/scenario-parameters.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/three-gpp-ftp-m1-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-stats-calculator.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/helper/nr-mac-scheduling-stats.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-net-device.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-gnb-net-device.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-ue-net-device.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-phy.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-gnb-phy.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-ue-phy.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-spectrum-phy.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-interference.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-pdu-info.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-vs.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-vs-ul.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-vs-dl.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-fs.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-fs-ul.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-header-fs-dl.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-short-bsr-ce.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-phy-mac-common.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-tdma-rr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-tdma-pf.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ofdma-rr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ofdma-pf.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-tdma-qos.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ofdma-qos.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-control-messages.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-spectrum-signal-parameters.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-radio-bearer-tag.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-amc.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-sched-sap.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-csched-sap.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-phy-sap.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-lte-mi-error-model.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-gnb-mac.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-ue-mac.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-rrc-protocol-ideal.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-harq-phy.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/bandwidth-part-gnb.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/bandwidth-part-ue.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/bwp-manager-gnb.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/bwp-manager-ue.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/bwp-manager-algorithm.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-harq-process.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-harq-vector.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-harq-rr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-cqi-management.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-lcg.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ns3.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-tdma.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ofdma.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ofdma-mr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-tdma-mr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ue-info.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ue-info-mr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ue-info-rr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ue-info-pf.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-ue-info-qos.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-lc-alg.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-lc-rr.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-lc-qos.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-error-model.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-t1.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-t2.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-ir.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-cc.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-ir-t1.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-ir-t2.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-cc-t1.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-eesm-cc-t2.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-error-model.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-ch-access-manager.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/beam-id.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/beamforming-vector.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/beam-manager.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/beamforming-algorithm.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/ideal-beamforming-algorithm.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/realistic-beamforming-algorithm.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/sfnsf.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/lena-error-model.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-srs.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mac-scheduler-srs-default.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-ue-power-control.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/realistic-bf-manager.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/distance-based-three-gpp-spectrum-propagation-loss-model.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-ftp-single.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-ftp-multi.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-video.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-gaming.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-ngmn-voip.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-pose-control.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-audio-data.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/model/traffic-generator-3gpp-generic-video.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/helper/traffic-generator-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/utils/traffic-generators/helper/xr-traffic-mixer-helper.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-cb-two-port.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-cb-type-one.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mimo-chunk-processor.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mimo-matrices.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-pm-search-full.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-pm-search.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/contrib/nr/model/nr-mimo-signal.h"
    "/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/build/include/ns3/nr-module.h"
    )
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/jarvis/Documents/ns-allinone-3.42/ns-3.42/cmake-cache/contrib/nr/examples/cmake_install.cmake")

endif()

