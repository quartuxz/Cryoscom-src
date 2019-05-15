import math

def rotate(origin, point, angle):
    """
    Rotate a point counterclockwise by a given angle around a given origin.

    The angle should be given in radians.
    """
    ox, oy = origin
    px, py = point

    qx = ox + math.cos(angle) * (px - ox) - math.sin(angle) * (py - oy)
    qy = oy + math.sin(angle) * (px - ox) + math.cos(angle) * (py - oy)
    return qx, qy

def AIBehaviour(playerPos, playerRadius, xPlayerVel, yPlayerVel,
                canFire,
                xThisPos, yThisPos, thisRadius, bulletSpeed,
                hasHead, xHeadPos, yHeadPos, headRadius, xDefaultDistance, yDefaultDistance):
    origin = (xThisPos, yThisPos)
    point = (playerPos[0][0], playerPos[0][1])

    rx, ry = rotate(origin, point, math.pi)
    return(rx, ry, 1, playerPos[0][0], playerPos[0][1],True)
