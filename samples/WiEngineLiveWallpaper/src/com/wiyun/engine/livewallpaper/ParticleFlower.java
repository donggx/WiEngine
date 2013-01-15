/*
 * Copyright (c) 2010 WiYun Inc.
 * Author: luma(stubma@gmail.com)
 *
 * For all entities this program is free software; you can redistribute
 * it and/or modify it under the terms of the 'WiEngine' license with
 * the additional provision that 'WiEngine' must be credited in a manner
 * that can be be observed by end users, for example, in the credits or during
 * start up. (please find WiEngine logo in sdk's logo folder)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
package com.wiyun.engine.livewallpaper;

import com.wiyun.engine.opengl.Texture2D;
import com.wiyun.engine.particle.ParticleSystem;
import com.wiyun.engine.particle.QuadParticleSystem;

public class ParticleFlower extends QuadParticleSystem {
    public static ParticleSystem make() {
        return new ParticleFlower();
    }

    protected ParticleFlower() {
        this(250);
    }

    protected ParticleFlower(int p) {
        super(p);

        // duration
        setDuration(1f);

        // angle
        setDirectionAngleVariance(90, 360);

        // speed of particles
        setSpeedVariance(80, 10);

        // radial
        setRadialAccelerationVariance(-60, 0);

        // tagential
        setTangentialAccelerationVariance(15, 0);

        // life of particles
        setLifeVariance(4, 1);

        // size, in pixels
        setStartSizeVariance(30f, 10f);

        // emits per second
        setEmissionRate(getMaxParticleCount() / getLife());

        // color of particles
        setStartColorVariance(0.5f, 0.5f, 0.5f, 1f, 0.5f, 0.5f, 0.5f, 0.5f);
        setEndColorVariance(0f, 0f, 0f, 1f, 0f, 0f, 0f, 0f);

        setTexture(Texture2D.make(R.drawable.stars));

        // additive
        setBlendAdditive(true);
    }

}
