/*
 * @(#)____SMARCHBOX__COOLSAND__V1_0_0__19____.h 0.100 2008-12-05
 *
 * Copyright 2007 Smarch.
 * All rights reserved.
 */

///////////////////////////////////////////////////////////// 图形模块 /////////////////////////////////////////////////////////////
//外部定义
//图形实现过程中，必须要实现一套快速写缓存的方法。
//对于 16bit 每像素来说，只要实现如下方法即可：
/**
 * 获得字体字模相关信息。
 * @param Ch 查询的字符
 * @param Font 字体属性
 * @param ppCharData 字体数据存储位置
 * @param pnWidth 字体宽度
 * @param pnHeight 字体高度
 * @param pdataWidth 字体数据宽度(暂不使用)
 * @param pdataHeight 字体数据高度(暂不使用)
 * @param ascent 字体上基准线
 * @param descent 字体下基准线
 * @return 字体点阵数据占的字节数
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
//jmetoc_getCharBitmap  //本方法在移植模块中定义

/**
 * 设置缓存中的某部分存储点。
 * @param address 缓存存储
 * @param pixel 像素颜色
 * @param size 设置存储大小，顺序的一块存储
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中绘制圆弧。
 * @param dstPtr 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x 绘制的 X 坐标
 * @param y 绘制的 Y 坐标
 * @param width 圆弧宽度
 * @param height 圆弧高度
 * @param startAngle 圆弧其实弧度
 * @param arcAngle 圆弧绘制的弧度
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @param dotted 是否绘制点线
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中绘制直线。
 * @param imgp 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x1 绘制起点的 X 坐标
 * @param y1 绘制起点的 Y 坐标
 * @param x2 绘制终点的 X 坐标
 * @param y2 绘制终点的 Y 坐标
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @param dotted 是否绘制点线
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中绘制矩形。
 * @param imgp 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x1 绘制起点的 X 坐标
 * @param y1 绘制起点的 Y 坐标
 * @param x2 绘制终点的 X 坐标
 * @param y2 绘制终点的 Y 坐标
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @param dotted 是否绘制点线
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中填充圆弧。
 * @param dstPtr 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x 绘制的 X 坐标
 * @param y 绘制的 Y 坐标
 * @param width 圆弧宽度
 * @param height 圆弧高度
 * @param startAngle 圆弧其实弧度
 * @param arcAngle 圆弧绘制的弧度
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中填充矩形。
 * @param imgp 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x1 绘制起点的 X 坐标
 * @param y1 绘制起点的 Y 坐标
 * @param x2 绘制终点的 X 坐标
 * @param y2 绘制终点的 Y 坐标
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 在缓存中填充三角形。
 * @param imgp 缓存存储
 * @param destWidth 缓存区域宽度
 * @param x1 三角形第一个点的 X 坐标
 * @param y1 三角形第一个点的 Y 坐标
 * @param x2 三角形第二个点的 X 坐标
 * @param y2 三角形第二个点的 Y 坐标
 * @param x3 三角形第三个点的 X 坐标
 * @param y3 三角形第三个点的 Y 坐标
 * @param clipx1 绘制控制区域左上点 X
 * @param clipy1 绘制控制区域左上点 Y
 * @param clipx2 绘制控制区域右下点 X
 * @param clipy2 绘制控制区域右下点 Y
 * @param color 颜色
 * @param dotted 是否绘制点线
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 设置字体信息。
 * @param Font 字体属性
 * @param arrCount 保留
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 获得字符的宽度和高度。
 * @param Ch 字符
 * @param pWidth 宽度存储的变量
 * @param pHeight 高度存储的变量
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 获得字符的高度。
 * @return 字符高度
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 

/**
 * 获得字符的基准线。
 * @return 字符基准线
 * @author runasea
 * @version 1.0.0.1, 2009-01-02
 */
 
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
