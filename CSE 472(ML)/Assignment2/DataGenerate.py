import numpy as np
from scipy.stats import chi2
import matplotlib.pyplot as plt
from matplotlib.patches import Ellipse



def generate_sample(weight=None,size=None):

    mean={"one":[2.5,0.5],"two":[0.5,3],"three":[3,8]}
    cov={"one":[[0.5, 0.2],[0.2, 0.3]],"two":[[0.267, 0.2],[0.2, 0.438]],"three":[[0.5, 0],[0, 0.5]]}

    if size is None:
        size=1000
    if weight is None:
        weight=[0.23,0.35,0.42]
    np.random.seed(seed=35)
    sample=np.random.choice(["one","two","three"],size=size,p=weight)
    #print(sample[0:20])
    data=np.empty(shape=[2,0])
    for item in sample:
        data=np.concatenate((data,np.random.multivariate_normal(mean[item], cov[item], 1).T),axis=1)
    print(type(data))
    return data


def plot_cov_ellipse(cov, pos, volume=.5, ax=None, fc='none', ec=[0,0,0], a=1, lw=2):
    """
    Plots an ellipse enclosing *volume* based on the specified covariance
    matrix (*cov*) and location (*pos*). Additional keyword arguments are passed on to the
    ellipse patch artist.
    Parameters
    ----------
        cov : The 2x2 covariance matrix to base the ellipse on
        pos : The location of the center of the ellipse. Expects a 2-element
            sequence of [x0, y0].
        volume : The volume inside the ellipse; defaults to 0.5
        ax : The axis that the ellipse will be plotted on. Defaults to the
            current axis.
    """



    def eigsorted(cov):
        vals, vecs = np.linalg.eigh(cov)
        order = vals.argsort()[::-1]
        return vals[order], vecs[:,order]

    if ax is None:
        ax = plt.gca()

    vals, vecs = eigsorted(cov)
    theta = np.degrees(np.arctan2(*vecs[:,0][::-1]))

    kwrg = {'facecolor':fc, 'edgecolor':ec, 'alpha':a, 'linewidth':lw}

    # Width and height are "full" widths, not radius
    width, height = 2 * np.sqrt(chi2.ppf(volume,2)) * np.sqrt(vals)
    ellip = Ellipse(xy=pos, width=width, height=height, angle=theta, **kwrg)

    ax.add_artist(ellip)

