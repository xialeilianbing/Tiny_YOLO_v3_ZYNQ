#include "batchnorm_layer.h"

void forward_batchnorm_layer(layer l, network net)
{
//    if(l.type == BATCHNORM) copy_cpu(l.outputs*l.batch, net.input, 1, l.output, 1);
//    copy_cpu(l.outputs*l.batch, l.output, 1, l.x, 1);
    //if(net.train){
    //    mean_cpu(l.output, l.batch, l.out_c, l.out_h*l.out_w, l.mean);
    //    variance_cpu(l.output, l.mean, l.batch, l.out_c, l.out_h*l.out_w, l.variance);
    //
    //    scal_cpu(l.out_c, .99, l.rolling_mean, 1);
    //    axpy_cpu(l.out_c, .01, l.mean, 1, l.rolling_mean, 1);
    //    scal_cpu(l.out_c, .99, l.rolling_variance, 1);
    //    axpy_cpu(l.out_c, .01, l.variance, 1, l.rolling_variance, 1);
    //
    //    normalize_cpu(l.output, l.mean, l.variance, l.batch, l.out_c, l.out_h*l.out_w);
    //    copy_cpu(l.outputs*l.batch, l.output, 1, l.x_norm, 1);
    //} else {
//        normalize_cpu(l.output, l.rolling_mean, l.rolling_variance, l.batch, l.out_c, l.out_h*l.out_w);
    //}
//    scale_bias(l.output, l.scales, l.batch, l.out_c, l.out_h*l.out_w);
//    add_bias(l.output, l.biases, l.batch, l.out_c, l.out_h*l.out_w);
}