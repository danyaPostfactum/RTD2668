del hex\sparrow.H00
del hex\sparrow.H01
del hex\sparrow.H02
del hex\sparrow.H03
del hex\sparrow.H04
del hex\sparrow.H05
del hex\sparrow.H06
del hex\sparrow.H07
del hex\sparrow.H08
@echo off
copy output\sparrow.H00 hex\sparrow.H00
copy output\sparrow.H01 hex\sparrow.H01
copy output\sparrow.H02 hex\sparrow.H02
copy output\sparrow.H03 hex\sparrow.H03
copy output\sparrow.H04 hex\sparrow.H04
copy output\sparrow.H05 hex\sparrow.H05
copy output\sparrow.H06 hex\sparrow.H06
copy output\sparrow.H07 hex\sparrow.H07
copy output\sparrow.H08 hex\sparrow.H08
HEX2BIN /Q /L589824   hex\sparrow.H00 hex\RTD2668.BIN
HEX2BIN /Q /O65536 /M hex\sparrow.H01 hex\RTD2668.BIN
HEX2BIN /Q /O131072 /M hex\sparrow.H02 hex\RTD2668.BIN
HEX2BIN /Q /O196608 /M hex\sparrow.H03 hex\RTD2668.BIN
HEX2BIN /Q /O262144 /M hex\sparrow.H04 hex\RTD2668.BIN
HEX2BIN /Q /O327680 /M hex\sparrow.H05 hex\RTD2668.BIN
HEX2BIN /Q /O393216 /M hex\sparrow.H06 hex\RTD2668.BIN
HEX2BIN /Q /O458752 /M hex\sparrow.H07 hex\RTD2668.BIN
HEX2BIN /Q /O524288 /M hex\sparrow.H08 hex\RTD2668.BIN