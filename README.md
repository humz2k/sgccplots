# sgccplots
 
Simple plotting library written in sugaryc (https://github.com/humz2k/sugaryc).

Example:
```
#include <stdlib.sgh>
#include <assert.sgh>
#include "include/sgccplots.sgh"
#include <range.sgh>
#include <math.sgh>
#include <filter_map_reduce.sgh>

std.list<double> even_sample(double min, double max, int n){
    double step = (max-min)/(double)n;
    auto out = std.make_list<double>();
    foreach(auto i in range(n)){
        out.append(min + step*i);
    }
    return out;
}

int main(){

    auto xs = even_sample(-10,10,100);

    auto ys = std.map<std.list<double>,val*val*val>(xs);
    auto ys2 = std.map<std.list<double>,val*val>(xs);
    auto ys3 = std.map<std.list<double>,1/(sqrtf(fabs(val))+1)>(xs);
    auto ys4 = std.map<std.list<double>,(sin(val))>(xs);

    //auto plt = make_plot(1000,500,min(xs),min(ys),max(xs),max(ys),50,50);
    auto plt = make_plot(500,500,-10,-10,10,10,0,0);

    plt.axes(gThickness(0.05));

    plt.line(xs,ys2,gThickness(0.1) + gColor("orange"));
    plt.scatter(xs,ys2,gRadius(0.1) + gColor("orange"));

    plt.line(xs,xs,gThickness(0.1) + gColor("green"));
    plt.scatter(xs,xs,gRadius(0.1) + gColor("green"));

    plt.line(xs,ys,gThickness(0.1) + gColor("blue"));
    plt.scatter(xs,ys,gRadius(0.1) + gColor("blue"));

    plt.line(xs,ys3,gThickness(0.1) + gColor("red"));
    plt.scatter(xs,ys3,gRadius(0.1) + gColor("red"));

    plt.line(xs,ys4,gThickness(0.1) + gColor("skyblue"));
    plt.scatter(xs,ys4,gRadius(0.1) + gColor("skyblue"));

    plt.show();

    return 0;
}
```