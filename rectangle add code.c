void ractangle(int x, int y, int xx, int yy){
    //drawLineDDA(x,y, x+xx, y);
    clipLine(winMin, winMax, {x,y},{x+xx,y} );
    //drawLineDDA(x+xx,y, x+xx, y+yy);
    clipLine(winMin, winMax, {x+xx,y},{x+xx,y+yy} );
    //drawLineDDA(x,y+yy, x+xx, y+yy);
    clipLine(winMin, winMax, {x,y+yy},{x+xx,y+yy} );
    //drawLineDDA(x,y+yy, x, y);
    clipLine(winMin, winMax, {x,y+yy},{x,y} );
}
