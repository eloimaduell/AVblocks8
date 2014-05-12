//
//  AVparameters.h
//  ignasiAVsynth_0003
//
//  Written by ignasi on 04/01/13.
//
//

#ifndef ignasiAVsynth_0003_AVparameters_h
#define ignasiAVsynth_0003_AVparameters_h

 /* parameters, the VST style ... */

enum{
    
    masterPower,//0
    
    unused_01,//1

    /* OSC1 */
    
    isPlayingSAW,//2
    isPlayingTRI,//3
    isPlayingSQU,//4
    isPlayingPULSE,//5
    isPlayingNOISE,//6
    isPlayingSINBUF,//7
    kPWM,//8
    
    unused_09,//9
    
    kPanSAW,//10
    kPanTRI,//11
    kPanSQU,//12
    kPanPULSE,//13
    kPanNOISE,//14
    kPanSINBUF,//15
    
    kVolSIN,//16
    kVolSAW,//17
    kVolTRI,//18
    kVolSQU,//19
    kVolPULSE,//20
    kVolNOISE,//21
    
    kHarmSIN,//22
    kHarmSAW,//23
    kHarmTRI,//24
    kHarmSQU,//25
    kHarmPULSE,//26
    
    unused_27,//27
    unused_28,//28
    unused_29,//29
    
    /* ENV 1 */
    A1,//30
    D1,//31
    S1,//32
    R1,//33
    
    /* ENV 2 */
    A2,//34
    D2,//35
    S2,//36
    R2,//37
    
    kEnv2ToFilter,//38
    
    unused_39,//39
    
    /*FILTER 1 */
    
    isFilter1Playing,//40
    kFilter1Frequency,//41
    kFilter1Resonance,//42
    
    unused_43,//43
    unused_44,//44
    unused_45,//45
    unused_46,//46
    unused_47,//47
    unused_48,//48
    unused_49,//49
    
    /* LFO */
    
    isLfoPlaying,//50
    kWaveformLfo,//51
    kLFOmode,//52
    kLFOfreq,//53
    klfoModPitchSINBUFamount,//54
    klfoModPitchTRIamount,//55
    klfoModPitchSAWamount,//56
    klfoModPitchSQUamount,//57
    klfoModPitchPWMamount,//58
    klfoModPulsePWMamount,//59
    klfoOffset,//60
    unused_61,//61
    unused_62,//62
    unused_63,//63
    unused_64,//64
    unused_65,//65
    unused_66,//66
    unused_67,//67
    unused_68,//68
    unused_69,//69
    unused_70,//70
    unused_71,//71
    unused_72,//72
    unused_73,//73
    unused_74,//74
    unused_75,//75
    unused_76,//76
    unused_77,//77
    unused_78,//78
    unused_79,//79
    unused_80,//80
    unused_81,//81
    unused_82,//82
    unused_83,//83
    unused_84,//84
    unused_85,//85
    unused_86,//86
    unused_87,//87
    unused_88,//88
    unused_89,//89
    unused_90,//90
    unused_91,//91
    unused_92,//92
    unused_93,//93
    unused_94,//94
    unused_95,//95
    unused_96,//96
    unused_97,//97
    unused_98,//98
    unused_99,//99
    
    /* SAMPLER */
    setPlaySAMPLER,//100
    LoopMode,//101
    
    unused_102,//102
    unused_103,//103
    unused_104,//104
    unused_105,//105
    unused_106,//106
    unused_107,//107
    unused_108,//108
    unused_109,//109
    unused_110,//110
    unused_111,//111
    unused_112,//112
    unused_113,//113
    unused_114,//114
    unused_115,//115
    unused_116,//116
    unused_117,//117
    unused_118,//118
    unused_119,//119
    unused_120,//120
    unused_121,//121
    unused_122,//122
    unused_123,//123
    unused_124,//124
    unused_125,//125
    unused_126,//126
    unused_127,//127
    unused_128,//128
    unused_129,//129
    unused_130,//130
    unused_131,//131
    unused_132,//132
    unused_133,//133
    unused_134,//134
    unused_135,//135
    unused_136,//136
    unused_137,//137
    unused_138,//138
    unused_139,//139
    unused_140,//140
    unused_141,//141
    unused_142,//142
    unused_143,//143
    unused_144,//144
    unused_145,//145
    unused_146,//146
    unused_147,//147
    unused_148,//148
    unused_149,//149
    unused_150,//150
    unused_151,//151
    unused_152,//152
    unused_153,//153
    unused_154,//154
    unused_155,//155
    unused_156,//156
    unused_157,//157
    unused_158,//158
    unused_159,//159
    unused_160,//160
    unused_161,//161
    unused_162,//162
    unused_163,//163
    unused_164,//164
    unused_165,//165
    unused_166,//166
    unused_167,//167
    unused_168,//168
    unused_169,//169
    unused_170,//170
    unused_171,//171
    unused_172,//172
    unused_173,//173
    unused_174,//174
    unused_175,//175
    unused_176,//176
    unused_177,//177
    unused_178,//178
    unused_179,//179
    unused_180,//180
    unused_181,//181
    unused_182,//182
    unused_183,//183
    unused_184,//184
    unused_185,//185
    unused_186,//186
    unused_187,//187
    unused_188,//188
    unused_189,//189
    unused_190,//190
    unused_191,//191
    unused_192,//192
    unused_193,//193
    unused_194,//194
    unused_195,//195
    unused_196,//196
    unused_197,//197
    unused_198,//198
    unused_199,//199
    
    
    // parameter number >200 to be used on effects (not voices)
    
    /* Reverb */
    kReverbRoomSize,//200
    kReverbDamp,//201
    kReverbWet,//202
    isPlayingREVERB,//203
    kReverbwidth,//204
    
    unused_205,//205
    unused_206,//206
    unused_207,//207
    unused_208,//208
    unused_209,//209
    
    /* Distor */
   
    setPlayDistor,//210
    setLevelDistor,//211
    setGainDistor,//212
    setDistorMode,//213
    
    unused_214,//214
    unused_215,//215
    unused_216,//216
    unused_217,//217
    unused_218,//218
    unused_219,//219
    unused_220,//220
    unused_221,//221
    unused_222,//222
    unused_223,//223
    unused_224,//224
    unused_225,//225
    unused_226,//226
    unused_227,//227
    unused_228,//228
    unused_229,//229
    unused_230,//230
    unused_231,//231
    unused_232,//232
    unused_233,//233
    unused_234,//234
    unused_235,//235
    unused_236,//236
    unused_237,//237
    unused_238,//238
    unused_239,//239
    unused_240,//240
    unused_241,//241
    unused_242,//242
    unused_243,//243
    unused_244,//244
    unused_245,//245
    unused_246,//246
    unused_247,//247
    unused_248,//248
    unused_249,//249
    unused_250,//250    
    
    kNumParams
};

#endif
