FROM continuumio/anaconda3:5.0.1

# install sys requirements
RUN apt-get update
RUN apt-get install -y build-essential git

# clone in repo
RUN git clone https://github.com/DerThorsten/zgraph

# update conda
RUN conda update --yes conda

# go into repo
RUN cd zgraph && conda env create -f zgraph-dev-requirements.yml

# activate
RUN echo "source activate zgraph-dev-requirements" > ~/.bash
ENV PATH /opt/conda/envs/zgraph-dev-requirements/bin:$PATH

# activate
#RUN conda activate zgraph-dev-requirements

# make build dir
RUN cd zgraph && mkdir build

# run cmake
RUN  cd zgraph/build && \
    cmake .. -DCMAKE_INSTALL_PREFIX=$(conda info --base)/envs/zgraph-dev-requirements

# build and install project
RUN cd zgraph/build && \
    make -j2 install