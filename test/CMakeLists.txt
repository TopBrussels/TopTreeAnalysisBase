find_package(GTest REQUIRED)

find_package(ROOT REQUIRED dl Core RIO Net Hist Graf Graf3d Gpad Tree Rint Postscript Matrix Physics MathCore Thread Gui pthread m)
include(${ROOT_USE_FILE})

subdirs (testTools)
subdirs (testKinFitter)
